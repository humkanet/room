#include <xc.h>
#include "ldo.h"
#include "i2c.h"
#include "spi.h"
#include "clock.h"
#include "cc1101.h"
#include "shtc1.h"
#include "vbat.h"
#include "nvm.h"
#include "fnv1.h"
#include "main.h"
#include "config.h"


#define U8(x)  ((uint8_t) (x))


void interrupt isr()
{
	clock_isr();
	// Таймер ожидания
	if (TMR2IF){
		TMR2IF = 0;
	}
}


inline void setup()
{
	// Отключаем периферию
	PMD0   = 0xFF;
	PMD1   = 0xFF;
	PMD2   = 0xFF;
	PMD3   = 0xFF;
	PMD4   = 0xFF;
	PMD5   = 0xFF;
	// Тактирование периферии
	SYSCMD = 0;
	// Инициализируем модули
	ldo_init();
	clock_init();
	// Инициализируем радио
	spi_init();
	cc1101_init();
	cc1101_write_burst(CC1101_IOCFG2, (uint8_t *) rfSettings, sizeof(rfSettings));
	cc1101_write_burst(CC1101_PATABLE, (uint8_t *) paTable, sizeof(paTable));
	cc1101_strobe(CC1101_SPWD);
	spi_off();
	// Тактирование периферии
	SYSCMD = 1;
	// Разрешаем прерывания по изменению пина
	IOCIE = 1;    
}


uint32_t id2addr()
{
	uint8_t buf[9];
	nvm_read(DIA_MUI, buf, 9);
	return fnv1_hash(buf, 9);
}


bool transmit(PACKET *pkt, uint8_t retry)
{
	bool ret = false;
	// Настраиваем таймер паузы между пакетами
	if (retry>1){
		TMR2MD   = 0;
		asm("nop");
		T2CLKCON = 0b00000010;
		T2CON    = PKT_DELAY_PS<<4;
		T2HLT    = 0b00001000;
		T2PR     = (PKT_DELAY_PR/PKT_DELAY_DIV)-1;
		T2TMR    = 0;
		TMR2IF   = 0;
		TMR2IE   = 1;
	}
	// Очищаем буфер приема
	cc1101_strobe(CC1101_SFRX);	
	// Пробуем отослать
	while(retry--){
		// Отправляем данные
		cc1101_strobe(CC1101_SFTX);
		cc1101_write_burst(CC1101_FIFO, (uint8_t *) pkt, sizeof(PACKET));
		cc1101_strobe(CC1101_STX);
		cc1101_so_waith();
		cc1101_so_waitl();
		// Пауза перед повтором
		if (retry){
			// Отправляем радио в сон
			cc1101_strobe(CC1101_SPWD);
			// Выдерживаем паузу
			T2ON = 1;
			while(T2ON) SLEEP();
			// Будим радио
			cc1101_strobe(CC1101_SIDLE);
		}
	}
	// Отключаем таймер ожидания
	TMR2MD = 1;
	// Возвращам результат
	return ret;
}


void main()
{
	PACKET pkt;
    
	// Инициализация
	di();
	setup();
	ei();
      
	// Заполняем пакет
	pkt.length = sizeof(PACKET)-1;
	pkt.addr   = id2addr();
	pkt.seq    = 0;

	// Настраиваем сон
	IDLEN = 0;
    
	// Основной цикл
	uint8_t t = clock_sec()+2;
	for(;;){
		uint8_t t2 = clock_sec();
		// Интервал отсылки
		if (U8(t2-t)>=TRANSMIT_INTERVAL){
			t = t2;
			// Тактирование периферии
			SYSCMD = 0;
			// Включаем LDO
			ldo_on();
			// Пока стабилизируется LDO замеряем батарейку	
			pkt.vbat = vbat_read();
			// Измеряем температуру/влажность
			i2c_init();
			bool ready = shtc1_measure();
			// Отключаем LDO
			i2c_off();
			ldo_off();
			// Считываем влажность/температуру
			if (ready){
				pkt.temperature = shtc1_temperature();
				pkt.humidity    = shtc1_humidity();
			}
			else{
				pkt.temperature = 0;
				pkt.humidity    = 0;
			}
			// Будим радио
			spi_init();
			cc1101_strobe(CC1101_SIDLE);
			// Отсылаем данные
			pkt.seq ++;
			transmit(&pkt, 2);
			// Отключаем радио
			cc1101_strobe(CC1101_SPWD);
			spi_off();
			// Тактирование периферии
			SYSCMD = 1;
		}
		// Спим
		SLEEP();
	}
}
