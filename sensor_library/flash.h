#include <stm32f10x_flash.h>
#include "lcd_display.h"

FLASH_Status flashsil(int sector);
FLASH_Status flashyaz(uint32_t address , uint32_t data);
FLASH_Status flashyaz_adr(int adr_num , uint32_t data);
void oku_name(int adr_num);
FLASH_Status flashyaz_val(int adr_num , uint16_t temp , uint16_t hum);

FLASH_Status flashyaz_name(int adr_num , char name []);
FLASH_Status flash_yaz_byte(uint32_t address , uint8_t data);
FLASH_Status flashyaz_adrb(int adr_num , uint8_t data);

uint16_t oku_hum(int adr_num);
uint16_t oku_temp(int adr_num);