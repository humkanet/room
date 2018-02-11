#include <xc.h>
#include "spi.h"
#include "pre.h"


#define SCK_PORT   C
#define SCK_PIN    0
#define MOSI_PORT  C
#define MOSI_PIN   1
#define MISO_PORT  C
#define MISO_PIN   2


void spi_init()
{
	// Включаем MSSP
#ifdef MSSP1MD_bit
	MSSP1MD = 0;
	asm("nop");
#endif
	// Отключаем MSSP
	SSPEN = 0;
	// Настраиваем пины
	TRIS(SCK_PORT, SCK_PIN)   = 0;
	TRIS(MOSI_PORT, MOSI_PIN) = 0;
	TRIS(MISO_PORT, MISO_PIN) = 1;
	ANS(MOSI_PORT, MOSI_PIN)  = 0;
	ANS(MISO_PORT, MISO_PIN)  = 0;
	ANS(SCK_PORT, SCK_PIN)    = 0;
	PPS(SCK_PORT, SCK_PIN)    = 0x15;
	PPS(MOSI_PORT, MOSI_PIN)  = 0x16;
	SSP1CLKPPS = PPSIN(SCK_PORT, SCK_PIN);
	SSP1DATPPS = PPSIN(MISO_PORT, MISO_PIN);
	// Включаем MSSP
	SSP1STAT   = 0x40;
	SSP1ADD    = 0x00;
	SSP1CON2   = 0x00;
	SSP1CON3   = 0x00;
	SSP1CON1   = 0x20;
}


void spi_off()
{
	// Отключаем MSSP
	SSP1CON1 = 0x00;
	// Настраиваем пины
	PPS(SCK_PORT, SCK_PIN)    = 0x00;
	PPS(MOSI_PORT, MOSI_PIN)  = 0x00;
	TRIS(SCK_PORT, SCK_PIN)   = 1;
	TRIS(MOSI_PORT, MOSI_PIN) = 1;
	TRIS(MISO_PORT, MISO_PIN) = 1;
	ANS(SCK_PORT, SCK_PIN)    = 1;
	ANS(MOSI_PORT, MOSI_PIN)  = 1;
	ANS(MISO_PORT, MISO_PIN)  = 1;
	// Отключаем MSSP1
#ifdef MSSP1MD_bit
	MSSP1MD = 1;
#endif
}


uint8_t spi_xchg(uint8_t data)
{
	// Отправляем данные
	SSP1CON1bits.WCOL = 0;
	SSP1BUF = data;
	while(!SSP1STATbits.BF);
	// Возвращаем результат
	return SSP1BUF;
}
