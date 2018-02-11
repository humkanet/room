#include <xc.h>
#include "i2c.h"
#include "pre.h"


#define SDA_PORT  C
#define SDA_PIN   5
#define SCL_PORT  C
#define SCL_PIN   4


#define i2c_wait(mask)  while((SSP1CON2 & 0x1F) | (SSP1STAT & mask))


void i2c_init()
{
#ifdef MSSP1MD_bit
	MSSP1MD = 0;
	asm("nop");
#endif
	// Отключаем MSSP
	SSPEN      = 0;
	// Настраиваем пины
	ANS(SDA_PORT, SDA_PIN)  = 0;
	ANS(SCL_PORT, SCL_PIN)  = 0;
	TRIS(SDA_PORT, SDA_PIN) = 1;
	TRIS(SCL_PORT, SCL_PIN) = 1;
	WPU(SDA_PORT, SDA_PIN)  = 0;
	WPU(SCL_PORT, SCL_PIN)  = 0;
	PPS(SDA_PORT, SDA_PIN)  = 0x16;
	PPS(SCL_PORT, SCL_PIN)  = 0x15;
	SSP1CLKPPS = 0x14;
	SSP1DATPPS = 0x15;
	// Настраиваем MSSP
	SSP1ADD     = _XTAL_FREQ/(4*I2C_SPEED)-1;
	SSP1STAT    = 0x80;
	SSP1CON2    = 0x00;
	SSP1CON3    = 0x00;
	SSP1CON1    = 0x28;
}


void i2c_off()
{
	// Отключаем MSSP
	SSP1CON1  = 0x00;
	// Настраиваем пины
	TRIS(SDA_PORT, SDA_PIN) = 1;
	TRIS(SDA_PORT, SDA_PIN) = 1;
	ANS(SDA_PORT, SDA_PIN)  = 1;
	ANS(SCL_PORT, SCL_PIN)  = 1;
	WPU(SDA_PORT, SDA_PIN)  = 0;
	WPU(SCL_PORT, SCL_PIN)  = 0;
	// Отключаем MSSP1
#ifdef MSSP1MD_bit
	MSSP1MD = 1;
#endif
}


bool i2c_start(uint8_t addr)
{
	// START
	SSP1CON2bits.SEN = 1;
	while(SSP1CON2bits.SEN);
	// Отправляем адрес устройства
	i2c_write(addr);
	return SSP1CON2bits.ACKSTAT==0;
}


bool i2c_restart(uint8_t addr)
{
	// RE-START
	SSP1CON2bits.RSEN = 1;
	while (SSP1CON2bits.RSEN);
	// Отправляем адрес устройства
	i2c_write(addr);
	return SSP1CON2bits.ACKSTAT==0;
}


void i2c_stop()
{
	SSP1CON2bits.PEN = 1;
	while(SSP1CON2bits.PEN);
}


void i2c_write(uint8_t val)
{
	SSP1BUF = val;
	while (SSP1STATbits.BF);
	i2c_wait(0x04);
}


void i2c_write_data(uint8_t *data, uint8_t len)
{
	while(len--){
		i2c_write(*data++);
	}
}


uint8_t i2c_read(bool ack)
{
	uint8_t val;
	SSP1CON2bits.RCEN = 1;
	while (!SSP1STATbits.BF);
	val    = SSP1BUF;
	i2c_wait(0x04);
	// ACK/nACK
	ACKDT  = ack ? 0 : 1;
	ACKEN  = 1;
	while (SSP1CON2bits.ACKEN);
	return val;
}


void i2c_read_data(uint8_t *data, uint8_t len)
{
	while (--len){
		*data++ = i2c_read(true);
	}
	*data = i2c_read(false);
}
