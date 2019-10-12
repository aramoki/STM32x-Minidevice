//. SHT11 LIBRARY ARAMOK
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_i2c.h>
#include <misc.h>


#define YELLOW 	GPIO_SetBits(GPIOD, GPIO_Pin_12);
#define ORANGE 	GPIO_SetBits(GPIOD, GPIO_Pin_13);
#define RED 		GPIO_SetBits(GPIOD, GPIO_Pin_14);
#define BLUE 		GPIO_SetBits(GPIOD, GPIO_Pin_15);

#define NYELLOW GPIO_ResetBits(GPIOD, GPIO_Pin_12)
#define NORANGE GPIO_ResetBits(GPIOD, GPIO_Pin_13)
#define NRED	 	GPIO_ResetBits(GPIOD, GPIO_Pin_14)
#define NBLUE 	GPIO_ResetBits(GPIOD, GPIO_Pin_15)


//SHT11 connections
#define SDA_pin_out_low 	GPIO_ResetBits(GPIOB , GPIO_Pin_13)
#define SDA_pin_out_high 	GPIO_SetBits(GPIOB , GPIO_Pin_13) 
#define SDA_pin_in 				GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_13)

#define SCL_pin_low 			GPIO_ResetBits(GPIOB , GPIO_Pin_12)
#define SCL_pin_high 			GPIO_SetBits(GPIOB , GPIO_Pin_12)





// SHT1x definitions
#define noACK 0
#define ACK   1
                            //adr  command  r/w
#define STATUS_REG_W 0x06   //000   0011    0
#define STATUS_REG_R 0x07   //000   0011    1
#define MEASURE_TEMP 0x03   //000   0001    1
#define MEASURE_HUMI 0x05   //000   0010    1
#define RESET        0x1e   //000   1111    0

void set_sda_inputmode(void);
void set_sda_outputmode(void);
void init(void);

void Delay(uint32_t delay);
void s_transstart(void);
unsigned char s_read_byte(unsigned char ack);
unsigned char s_write_byte(unsigned char value);
unsigned char s_measure(unsigned int *p_value, unsigned char mode);
float calc_sth11_temp(unsigned int t);
float calc_sth11_humi(unsigned int h, int t);
void Read_SHT11(float *fT, float *fRH,unsigned int *tnc , unsigned int *hnc );
char s_read_statusreg(unsigned char *p_value);
char s_write_statusreg(unsigned char value);
void s_connectionreset(void);
unsigned char s_softreset(void);


