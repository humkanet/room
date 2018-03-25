#ifndef _MAIN_H
#define	_MAIN_H

#include <stdint.h>


#define TRANSMIT_INTERVAL 30


#define PKT_DELAY_MS  7
#define PKT_DELAY_PR  (_LFINTOSC_FREQ*PKT_DELAY_MS/1000)
#if (PKT_DELAY_PR/1)<=256
	#define PKT_DELAY_DIV  1
	#define PKT_DELAY_PS   0b000
#elif (PKT_DELAY_PR/2)<=256
	#define PKT_DELAY_DIV  2
	#define PKT_DELAY_PS   0b001
#elif (PKT_DELAY_PR/4)<=256
	#define PKT_DELAY_DIV  4
	#define PKT_DELAY_PS   0b010
#elif (PKT_DELAY_PR/8)<=256
	#define PKT_DELAY_DIV  8
	#define PKT_DELAY_PS   0b011
#elif (PKT_DELAY_PR/16)<=256
	#define PKT_DELAY_DIV  16
	#define PKT_DELAY_PS   0b100
#elif (PKT_DELAY_PR/32)<=256
	#define PKT_DELAY_DIV  32
	#define PKT_DELAY_PS   0b101
#elif (PKT_DELAY_PR/64)<=256
	#define PKT_DELAY_DIV  64
	#define PKT_DELAY_PS   0b110
#elif (PKT_DELAY_PR/128)<=256
	#define PKT_DELAY_DIV  128
	#define PKT_DELAY_PS   0b111
#else
	#error "Invalid PKT_DELAY_MS"
#endif




const uint8_t paTable[8]   = {0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const uint8_t rfSettings[] = {
    0x2F,  // IOCFG2        GDO2 Output Pin Configuration
    0x06,  // IOCFG1        GDO1 Output Pin Configuration (0x29 = CHIP_RDYn, 0x23 = 3-state)
    0x2F,  // IOCFG0        GDO0 Output Pin Configuration
    0x07,  // FIFOTHR       RX FIFO and TX FIFO Thresholds
    0xD3,  // SYNC1         Sync Word, High Byte
    0x91,  // SYNC0         Sync Word, Low Byte
    0x20,  // PKTLEN        Packet Length
    0x04,  // PKTCTRL1      Packet Automation Control
    0x05,  // PKTCTRL0      Packet Automation Control
    0x00,  // ADDR          Device Address
    0x00,  // CHANNR        Channel Number
    0x0C,  // FSCTRL1       Frequency Synthesizer Control
    0x00,  // FSCTRL0       Frequency Synthesizer Control
    0x10,  // FREQ2         Frequency Control Word, High Byte
    0xB1,  // FREQ1         Frequency Control Word, Middle Byte
    0x3B,  // FREQ0         Frequency Control Word, Low Byte
    0x2D,  // MDMCFG4       Modem Configuration
    0x3B,  // MDMCFG3       Modem Configuration
    0x13,  // MDMCFG2       Modem Configuration
    0x22,  // MDMCFG1       Modem Configuration
    0xF8,  // MDMCFG0       Modem Configuration
    0x62,  // DEVIATN       Modem Deviation Setting
    0x07,  // MCSM2         Main Radio Control State Machine Configuration
//!!!
    0x30,  // MCSM1         0x30: rx->idle, tx->idle; 0x33: rx->idle, tx->rx
//!!!
    0x18,  // MCSM0         Main Radio Control State Machine Configuration
    0x1D,  // FOCCFG        Frequency Offset Compensation Configuration
    0x1C,  // BSCFG         Bit Synchronization Configuration
    0xC7,  // AGCCTRL2      AGC Control
    0x00,  // AGCCTRL1      AGC Control
    0xB0,  // AGCCTRL0      AGC Control
    0x87,  // WOREVT1       High Byte Event0 Timeout
    0x6B,  // WOREVT0       Low Byte Event0 Timeout
    0xFB,  // WORCTRL       Wake On Radio Control
    0xB6,  // FREND1        Front End RX Configuration
    0x10,  // FREND0        Front End TX Configuration
    0xEA,  // FSCAL3        Frequency Synthesizer Calibration
    0x2A,  // FSCAL2        Frequency Synthesizer Calibration
    0x00,  // FSCAL1        Frequency Synthesizer Calibration
    0x1F,  // FSCAL0        Frequency Synthesizer Calibration
    0x41,  // RCCTRL1       RC Oscillator Configuration
    0x00,  // RCCTRL0       RC Oscillator Configuration
    0x59,  // FSTEST        Frequency Synthesizer Calibration Control
    0x7F,  // PTEST         Production Test
    0x3F,  // AGCTEST       AGC Test
    0x88,  // TEST2         Various Test Settings
    0x31,  // TEST1         Various Test Settings
    0x09,  // TEST0         Various Test Settings
};


typedef struct {
	uint8_t   length;
	uint32_t  addr;
	uint8_t   seq;
	uint16_t  vbat;
	int16_t   temperature;
	uint16_t  humidity;
} PACKET;

#endif
