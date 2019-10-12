#include "lcd_display.h"


void init_lcd(void){
		GPIO_InitTypeDef GPIO_InitStruct1 , GPIO_InitStruct2;
		
	
		RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPBEN ,ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN ,ENABLE);
	
		GPIO_InitStruct1.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_14;
		GPIO_InitStruct1.GPIO_Mode = GPIO_Mode_Out_PP; 			// the pins are configured as alternate function so the USART peripheral has access to them
		GPIO_InitStruct1.GPIO_Speed = GPIO_Speed_10MHz;		// this defines the IO speed and has nothing to do with the baudrate!
		GPIO_Init(GPIOB, &GPIO_InitStruct1);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers
	
		GPIO_InitStruct2.GPIO_Pin = GPIO_Pin_7 |  GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
		//GPIO_InitStruct2.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_Out_PP;			// the pins are configured as alternate function so the USART peripheral has access to them
		GPIO_InitStruct2.GPIO_Speed = GPIO_Speed_10MHz;		// this defines the IO speed and has nothing to do with the baudrate!
		GPIO_Init(GPIOA, &GPIO_InitStruct2);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

	
}
void delay_us(int delay0)
{
	delay0 = delay0*4;
	while (delay0--)
	{	  
	}
}
void delay_ms(int delay1)
{
	delay1 = delay1*4000;
	while (delay1--)
	{ 
	}
}
void delay_s(int delay2)
{
	delay2 = delay2*4000000;
	while (delay2--)
	{ 
	}
}

void LCD_STROBE(void)
{
	_enable;
	delay_us(1);			//1us bekle
	_disable;
}

void data (unsigned char c)
{
	_datamode;
	delay_us(50);	//50us bekle
	databits = (c << 3) | (1 << 15); // c>>4
//	if(c & (1 << 7)){
//		 GPIO_SetBits(GPIOA , GPIO_Pin_11);
//	}else{
//			GPIO_ResetBits(GPIOA , GPIO_Pin_11);
//	}
	LCD_STROBE();
	databits = (c << 7) | (1 << 15); //c
//	if(c & (1 << 3)){
//			GPIO_SetBits(GPIOA , GPIO_Pin_11);
//	}else{
//			GPIO_ResetBits(GPIOA , GPIO_Pin_11);
//	}
	
	LCD_STROBE();
}

void cmd(unsigned char c)
{
	_commandmode;
	delay_us(50);
	databits = (c << 3) & ~(1 << 15);
//	if(c & (1 << 7)){
//		 GPIO_SetBits(GPIOA , GPIO_Pin_11);
//	}else{
//			GPIO_ResetBits(GPIOA , GPIO_Pin_11);
//	}
	LCD_STROBE();
	databits = (c << 7)  & ~(1 << 15);
//	if(c & (1 << 3)){
//			GPIO_SetBits(GPIOA , GPIO_Pin_11);
//	}else{
//			GPIO_ResetBits(GPIOA , GPIO_Pin_11);
//	}
	LCD_STROBE();
}
void clear(void)
{
	cmd(0x01);
	delay_ms(1);
}
void lcd_init()
{
	delay_ms(15);
	cmd(0x38);
	delay_ms(4);
	cmd(0x38);
	delay_us(100);
	cmd(0x38);
	cmd(0x28);
	cmd(0x28);
	cmd(0x0c);
	clear();
	cmd(0x6);
}
void string1(const char *q)
{
	while(*q)
		{
			data(*q++);
		}
}

void satir1(const char * q){
	cmd(0x80);
	string1(q);
}
void satir2(const char * q){
	cmd(0xC0);
	string1(q);
}
void satir3(const char * q){
	cmd(0x80 + 20);
	string1(q);
}
void satir4(const char * q){
	cmd(0xc0 + 20);
	string1(q);
}



void sayi(float dum1){
	int i,onuzeri[4]={1,10,100,1000};
	char display;
		cmd(0x80 + 0x14);
		for (i=0;i<4;i++){
			display = (char)((dum1)*onuzeri[i]);
			dum1=dum1-((float)(display)/onuzeri[i]);
			data(display+0x30);
			if(i==0)
				data(0x2E);
		}
}

void sayi2(int dum1){
	int i,onuzeri[4]={1,10,100,1000};
	char display;
		for (i=3;i>=0;i--){
			display = (char)((dum1 / onuzeri[i]) % 10 );
			data(display+0x30);
			if(i==2)
				data(0x2E);
		}
}

void sayi22(int dum1){
	int i,onuzeri[4]={1,10,100,1000};
	char display;
		for (i=3;i>=2;i--){
			display = (char)((dum1 / onuzeri[i]) % 10 );
			data(display+0x30);

		}
}

/*-----------------------LCD END-------------------*/
void test_lcd(void){
	char k,loc;
	delay_ms(50);
	lcd_init();

}

