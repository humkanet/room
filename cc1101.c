#include <xc.h>
#include "config.h"
#include "spi.h"
#include "cc1101.h"


#define cc1101_cs_clear()  LAT(CC1101_CS_PORT, CC1101_CS_PIN) = 0
#define cc1101_cs_set()    LAT(CC1101_CS_PORT, CC1101_CS_PIN) = 1


void cc1101_init()
{
	// Настраиваем пины
	TRIS(CC1101_CS_PORT, CC1101_CS_PIN) = 0;
	cc1101_cs_set();    
	// Сбрасываем чип
	cc1101_reset();
}


void cc1101_reset()
{
	// Сброс
	cc1101_cs_clear();
	cc1101_cs_set();
	__delay_us(50);
	cc1101_cs_clear();
	cc1101_so_waitl();
	// Отправляем команду сброса
	cc1101_strobe(CC1101_SRES);
	cc1101_so_waitl();
	cc1101_cs_set();
}


uint8_t cc1101_strobe(uint8_t strobe)
{
	// Выбираем cc1101
	cc1101_cs_clear();
	cc1101_so_waitl();
	// Отправлем команду
	spi_xchg(strobe);
	uint8_t res = spi_xchg(0);
	// Отменяем выбор
	cc1101_cs_set();
	// Состояние
	return res;
}


void cc1101_write(uint8_t addr, uint8_t b)
{
	// Выбираем cc1101
	cc1101_cs_clear();
	cc1101_so_waitl();
	// Отправляем адрес
	spi_xchg(addr | CC1101_WRITE);
	// Отправляем данные
	spi_xchg(b);
	// Отменяем выбор
	cc1101_cs_set();
}


void cc1101_write_burst(uint8_t addr, uint8_t *data, uint8_t len)
{
	// Выбираем cc1101
	cc1101_cs_clear();
	cc1101_so_waitl();
	// Отправляем адрес
	spi_xchg(addr | (CC1101_WRITE | CC1101_BURST));
	// Отправляем данные
	while(len--){
		spi_xchg(*data++);
	}
	// Отменяем выбор
	cc1101_cs_set();
}


uint8_t cc1101_read(uint8_t addr)
{
	// Выбираем cc1101
	cc1101_cs_clear();
	cc1101_so_waitl();
	// Отправляем адрес
	spi_xchg(addr | CC1101_READ);
	// Получаем данные
	uint8_t b = spi_xchg(0);
	// Отменяем выбор
	cc1101_cs_set();
	// Данные
	return b;
}


void cc1101_read_burst(uint8_t addr, uint8_t *data, uint8_t len)
{
	// Выбираем cc1101
	cc1101_cs_clear();
	cc1101_so_waitl();
	// Отправляем адрес
	spi_xchg(addr | (CC1101_READ | CC1101_BURST));
	// Считываем данные
	while(len--){
		*data++ = spi_xchg(0);
	}
	// Отменяем выбор
	cc1101_cs_set();
}
