#include <xc.h>
#include "i2c.h"
#include "shtc1.h"

#define U32(x)  ((uint32_t) (x))
#define I32(x)  ((int32_t) (x))

#define SHTC1_CRC_POLYNOMIAL  0x31
#define SHTC1_CRC_INIT        0xFF

const uint8_t SHTC1_CMD[][2] = {
	{0x80, 0x5D},  // Reset
	{0x60, 0x9C},  // LPM (blocking)
	{0x64, 0x58},  // LPM (non blocking)
	{0x78, 0x66},  // HPM (blocking)    
	{0x7C, 0xA2},  // HPM (non blocking)
	{0xEF, 0xC8},  // Read ID
};
#define SHTC1_CMD_RESET        0
#define SHTC1_CMD_MEASURE_LPM  1
#define SHTC1_CMD_MEASURE_HPM  3
#define SHTC1_CMD_READ_ID      5


typedef union {
	struct {
		uint8_t  low;
		uint8_t  high;
	};
	uint16_t  val;
} U16VAL;


static struct {
	U16VAL  temperature;
	U16VAL  humidity;
} shtc1;

bool shtc1_command(uint8_t cmd)
{
	// Начинаем обмен
	if (!i2c_start((SHTC1_ADDR<<1) | I2C_WRITE)){
		i2c_stop();
		return false;
	}
	// Передаем команду
	i2c_write_data((uint8_t *) SHTC1_CMD[cmd], 2);
	// Завершаем обмен
	i2c_stop();
	// Все нормально
	return true;
}


uint8_t shtc1_crc(uint8_t *data, uint8_t len)
{
	uint8_t crc = SHTC1_CRC_INIT;
	while(len--){
		crc ^= *data++;
		for (uint8_t n=0; n<8; n++){
			if (crc & 0x80){
				crc = (crc<<1) ^ SHTC1_CRC_POLYNOMIAL;
			}
			else{
				crc = (crc<<1);
			}
		}
	}
	return crc;
}


uint16_t shtc1_read_id()
{
	uint8_t data[3];
	// Read ID
	if (!shtc1_command(SHTC1_CMD_READ_ID)){
		return 0;
	}
	// Считываем значение
	i2c_start((SHTC1_ADDR<<1) | I2C_READ);
	i2c_read_data(data, sizeof(data));
	i2c_stop();
	// Проверяем CRC
	uint8_t crc = shtc1_crc(data, 2);
	if (crc!=data[2]) return 0;
	// Возвращаем ID    
	U16VAL id;
	id.high = data[0];
	id.low  = data[1];
	return id.val;
}


bool shtc1_measure()
{
	uint8_t data[6];
	// Отправляем команду на измерение
	if (!shtc1_command(SHTC1_CMD_MEASURE_HPM)){
		return false;
	}
	// Ждем
	__delay_us(SHTC1_MEASURE_HPM_US);
	// Считываем результат
	if (!i2c_start((SHTC1_ADDR<<1) | I2C_READ)){
		i2c_stop();
		return false;
	}
	i2c_read_data(data, sizeof(data));
	i2c_stop();
	// Проверяем CRC
	if ((shtc1_crc(data, 2)!=data[2]) || shtc1_crc(data+3, 2)!=data[5]){
		return false;
	}
	// Сохраняем значения
	shtc1.temperature.high = data[0];
	shtc1.temperature.low  = data[1];
	shtc1.humidity.high    = data[3];
	shtc1.humidity.low     = data[4];
	// Все нормально
	return true;
}


int16_t shtc1_temperature()
{
	int32_t val = I32(1750)*shtc1.temperature.val;
	val >>= 16;
	val -= 450;
	return val;    
}


uint16_t shtc1_humidity()
{
	uint32_t val = U32(1000)*shtc1.humidity.val;
	val >>= 16;
	return val;
}
