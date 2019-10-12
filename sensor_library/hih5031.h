#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_gpio.h>

//int msTicks;                      /* counts 1ms timeTicks       */

void SysTick_Handler(void);
void Delay_adc (int dly);
void init_ADC (void);
void Read_HIH5031(float *HIH5031_humm , float temp);