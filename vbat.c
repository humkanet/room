#include <xc.h>
#include "vbat.h"


uint16_t vbat_read()
{
	// Включаем периферию
	FVRMD    = 0;
	DAC1MD   = 0;
	ADCMD    = 0;
	// Настраиваем ИОН (1024мВ)
	FVRCON   = 0b10000001;
	// Настраиваем ЦАП (1/4 Vdd)
	DAC1CON0 = 0b10000000;
	DAC1CON1 = 32/4;
	// Настраиваем АЦП
	ADON     = 0;
	ADCON1   = 0b01110011;
	ADCON0   = 0b11110101;
	// Ждем стабилизации
	__delay_us(100);
	// Замеряем 4 раза
	uint16_t val = 0;
	for(uint8_t n=0; n<4; n++){
		GOnDONE = 1;
		while(GOnDONE);
		val += ADRESH;
	}
	// Отключаем периферию
	ADON     = 0;
	FVREN    = 0;
	DAC1EN   = 0;
	DAC1MD   = 1;
	ADCMD    = 1;
	FVRMD    = 1;
	/*
	* Расчитываем значение (2 знака после запятой):
	* Vbat/4  = 1024mV*(val/4)/256
	* Vbat/4  = 1024mv*val/1024
	* Vbat/4  = 1mV*val
	* Vbat    = 4mV*val
	* Vbat/10 = 4mV*val/10
	* Vbat    = 2mv*val/5
	*/
	val <<= 1;
	val = val/5;
	// Возвращаем значение
	return val;
}
