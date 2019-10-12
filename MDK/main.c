#include "stm32f10x.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_adc.h>

#include <stm32f10x_flash.h>

#include <string.h>
#include <stdio.h>
#include <math.h>


#include "../sensor_library/sht11.h"
#include "../sensor_library/hih5031.h"
#include "../sensor_library/lcd_display.h"
#include "../sensor_library/flash.h"
#include "../sensor_library/armenu.h"
#include "../sensor_library/resetseeker.h"

//float SHT11_temp , SHT11_humm;
float HIH5031_humm;
float dum1 , dum2 , HIH5031_humm2;
int error ;
//int selected_kayit;
//char name_tosave[16];
//rotator
int seek = 0;
int * seekptr = &seek;
int press = 0;

int press_again = 0;

 uint32_t address = 0x08010000;
  uint32_t datadat;
  uint32_t cnt = 0;
char str[20] , veri[20];

char * menu;




FLASH_Status FLASHStatus;
GPIO_InitTypeDef GPIO_InitStruct ,  GPIO_InitStruct22 , GPIO_InitStruct2;


void kayitgoster(int seek){
				satir1("Kayit:");
				sayi2(seek);	
				
				oku_name(seek);
					
				satir3("hum");
				sayi2(oku_hum(seek));
					
				
				satir4("temp");
				sayi2(oku_temp(seek));
}

void resetseek(){
	(*seekptr) = 0;
}



void setseek(int seek){
	seek = 0;
}
int timer = 0;
void SysTick_Handler(void)
{
	timer++;
} 

int gettimer(void){
	return timer;
}
void resettimer(){
	timer = 0;
}
int main(void){
	
	int cursor = 0;
	int page = 0;
	
	//page0 : enter name
	//page1 : reading sensor
	//page2 : save
	
		init();				// init for sht11
		init_ADC();		// init for hih5031
		init_lcd();
		test_lcd();
		
	

//		while(1){
//		//satir1("Testing str1");
//		//satir2("satir 2");
//				satir1("Hum : %");
//				sayi2((int)(SHT11_humm * 100));
//				
//				satir2("Temp: %");
//				sayi2((int)(SHT11_temp * 100));
//				Read_SHT11(&SHT11_temp , &SHT11_humm);// continious read
//				
//	
//	}

		
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPBEN , ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			// input pull down
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;		// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	//RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN , ENABLE);
	GPIO_InitStruct2.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 ;
	GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_IN_FLOATING; 			// input pull down
	GPIO_InitStruct2.GPIO_Speed = GPIO_Speed_10MHz;		// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_Init(GPIOB, &GPIO_InitStruct2);

	SysTick_Config(SystemCoreClock / 5);
	//flashsur();
	
	
	//14 LER 2 OLDU

	
		while(1){
			
			////THESE ARE INPUT LISTENERS
			
			if((GPIOB->IDR & (1<<5))){
				press = 0;
				delay_ms(200);
				if((GPIOB->IDR & (1<<5))){
						press = 1;
						//clear();
						//satir1("Reading SHT1x Hum...");
						//satir2("Bekleyiniz please.");
						//Read_SHT11(&SHT11_temp , &SHT11_humm);
						//clear();
				}
			}else{
			press = 0;
			}
				
				if(GPIOB->IDR & (1<<0)){
						delay_ms(10);
						if(GPIOB->IDR & (1<<1)){
						seek++;
							//clear();
						}
				}
				
				if(GPIOB->IDR & (1<<1)){
						delay_ms(10);
					if(GPIOB->IDR & (1<<0)){ 
						seek-=1;
						//clear();
//						if(seek == 0){
//							seek+=43;
//						}
					}
				}
				
				
				
//////////////////MENU/////////////////////////////////////////////////////////				
	
				
				showmenu(seek,press,seekptr);
				
				
				
				
			
}
	
		
	
	
//	
//	
//		ADC_SoftwareStartConvCmd(ADC1, ENABLE); // hih5031 adc start conversion
//    while(1){
//			
//			
//			
//				Read_SHT11(&SHT11_temp , &SHT11_humm);	
//				Read_HIH5031(&HIH5031_humm,SHT11_temp);
//			
//    }

//		
}
		


	