#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_i2c.h>
#include <misc.h>

void init_lcd(void);
void delay_us(int delay0);
void delay_ms(int delay1);
void delay_s(int delay2);
void LCD_STROBE(void);
void data (unsigned char c);
void cmd(unsigned char c);
void clear(void);
void lcd_init();
void string1(const char *q);

void test_lcd(void);
void satir1(const char * q);
void satir2(const char * q);
void satir3(const char * q);
void satir4(const char * q);
void sayi(float sayi);
void sayi2(int sayi);
void sayi22(int sayi);

#define _datamode GPIO_SetBits(GPIOB , GPIO_Pin_10)  //rs:B0
#define _commandmode GPIO_ResetBits(GPIOB , GPIO_Pin_10)  //rs:B0
#define _enable GPIO_SetBits(GPIOB , GPIO_Pin_14)  //em:B1
#define _disable GPIO_ResetBits(GPIOB , GPIO_Pin_14)  //en:B1
#define databits GPIOA->ODR

