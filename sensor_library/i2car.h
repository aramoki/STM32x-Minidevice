#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_i2c.h>
#include <misc.h>


#define I2C_TIMEOUT_MAX 100000
#define MEM_DEVICE_WRITE_ADDR 0xA0
#define MEM_DEVICE_READ_ADDR 0xA1

#define CARCAR_ADDR 0x28 << 1


enum  eepromtype  {M2401,M2402,M2404,M2408,M2416,M2432,M2464,M24128,M24256,M24512};

void Delayi2c(__IO uint32_t); // __IO => Volatile
void init_I2C1(void);
uint8_t Write_24Cxx(uint16_t, uint8_t, uint8_t);
uint8_t Read_24Cxx(uint16_t, uint8_t);

uint8_t Write_CC2A(uint16_t Addr, uint8_t Data);
uint8_t Read_CC2A(uint16_t Addr);
