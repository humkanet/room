#include <xc.h>
#include "vbat.h"


uint16_t vbat_read()
{
	// Включаем периферию
	FVRMD    = 0;
	ADCMD    = 0;
	// Настраиваем ИОН (1024мВ)
	FVRCON   = 0x81;
	// Настраиваем АЦП (мерим FVR1 относительно Vcc)
	ADON     = 0;
	ADACT    = 0x00;
	ADCON1   = 0x70;
	ADCON0   = 0xF9;
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
	ADCMD    = 1;
	FVRMD    = 1;
	/*
	* Расчитываем значение (2 знака после запятой):
	* 4*1024mV = Vcc*adc/255
	* Vcc = 4*255*1024mV/adc
	* Vcc = 1044480/adc
	* переходим от тысячных в десятые доли напряжения (делим на 10)
	* Vcc = (1044480/adc)/10
	* Vcc = 104448/adc
	* значение не помещаетсяв uint16, поэтому делим все на 2
	* Vcc/2 = 52224/adc
	* Vcc = (52224/adc)*2
	* Vcc = (52224/adc)<<1
	*/
	val = ((uint16_t)52224)/val;
	val <<= 1;
	// Возвращаем значение
	return val;
}
