#include "sht11.h"
const float C1 = -4.0;		  // for 12 Bit
const float C2 = +0.0405;	 // for 12 Bit
const float C3 = -0.0000028;  // for 12 Bit
const float Temp1 = +0.01;	// for 14 Bit
const float Temp2 = +0.00008; // for 14 Bit
unsigned char ucSens_Error;
int iSHT_Temp;
int iSHT_Humi;

void set_sda_inputmode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct2;
	GPIO_InitStruct2.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_IPD;		// input pull down
	GPIO_InitStruct2.GPIO_Speed = GPIO_Speed_10MHz; // this defines the IO speed and has nothing to do with the baudrate!
	GPIO_Init(GPIOB, &GPIO_InitStruct2);
}

void set_sda_outputmode(void)
{
	GPIO_InitTypeDef GPIO_InitStruct2;
	GPIO_InitStruct2.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct2.GPIO_Mode = GPIO_Mode_Out_PP;  // the pins are configured as alternate function so the USART peripheral has access to them
	GPIO_InitStruct2.GPIO_Speed = GPIO_Speed_10MHz; // this defines the IO speed and has nothing to do with the baudrate!
	GPIO_Init(GPIOB, &GPIO_InitStruct2);
}

void init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPBEN, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;  // the pins are configured as alternate function so the USART peripheral has access to them
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz; // this defines the IO speed and has nothing to do with the baudrate!
	GPIO_Init(GPIOB, &GPIO_InitStruct);			   // now all the values are passed to the GPIO_Init() function which sets the GPIO registers

	set_sda_outputmode();
}

void Delay(uint32_t delay)
{
	delay *= 0x00000aff;
	while (delay--)
		;
}

void s_transstart()
{
	//Initial state
	//YELLOW;
	SDA_pin_out_high; //release DATA-line
					  //NBLUE;
	SCL_pin_low;	  // SCL Low

	Delay(1);
	//BLUE;
	SCL_pin_high;
	Delay(1);
	//NYELLOW;
	SDA_pin_out_low; // SDA low
	Delay(1);
	//NBLUE;
	SCL_pin_low;
	Delay(3);
	//BLUE;
	SCL_pin_high;
	Delay(1);
	//YELLOW;
	SDA_pin_out_high; //release DATA-line
	Delay(1);
	//NBLUE;
	SCL_pin_low;
}

unsigned char s_read_byte(unsigned char ack)
{
	unsigned char i = 0x80;
	unsigned char val = 0;

	//Initial state
	SDA_pin_out_high; //release DATA-line
	SCL_pin_low;	  // SCL Low

	set_sda_inputmode();
	while (i) //shift bit for masking
	{
		//YELLOW;
		SCL_pin_high; //clk for SENSI-BUS
		Delay(1);
		if (SDA_pin_in == 1)
		{
			//BLUE;
			val = (val | i); //read bit
		}
		else
		{
			//RED;
		}
		//NYELLOW;
		SCL_pin_low;
		Delay(1);
		i = (i >> 1);
		//NRED;
		//NBLUE;
	}
	set_sda_outputmode();
	if (ack)
	{
		//in case of "ack==1" pull down DATA-Line
		SDA_pin_out_low;
		//ORANGE;
		//YELLOW;
	}
	else
	{
		SDA_pin_out_high;
		//RED;
		//BLUE;
	}

	SCL_pin_high; //clk #9 for ack
	Delay(3);
	SCL_pin_low;
	Delay(1);

	SDA_pin_out_high; //release DATA-line
	return (val);
}

unsigned char s_write_byte(unsigned char value)
{
	unsigned char i = 0x80;
	unsigned char error = 0;

	while (i)
	{
		//RED;
		if (i & value)
		{
			//BLUE;
			SDA_pin_out_high;
		}
		else
		{
			//YELLOW;
			SDA_pin_out_low;
		}

		SCL_pin_high;
		Delay(3);
		SCL_pin_low;
		//NRED;
		//NBLUE;
		//NYELLOW;
		Delay(3);
		i = (i >> 1);
	}

	SDA_pin_out_high;

	SCL_pin_high;
	Delay(3);
	set_sda_inputmode();
	if (SDA_pin_in == 1)
		error = 1;
	set_sda_outputmode();
	Delay(1);
	SCL_pin_low;

	return (error);
}

unsigned char s_measure(unsigned int *p_value, unsigned char mode)
{
	unsigned char i = 0;
	unsigned char msb, lsb;
	unsigned char checksum;

	*p_value = 0;
	s_transstart();

	if (mode)
	{
		mode = MEASURE_HUMI;
	}
	else
	{
		mode = MEASURE_TEMP;
	}

	if (s_write_byte(mode))
		return (1);

	SDA_pin_out_high;
	set_sda_inputmode();
	while (i < 240)
	{
		//NRED;
		//NBLUE;
		//NORANGE;
		//NYELLOW;
		Delay(10);
		Delay(10);
		Delay(10);
		if (SDA_pin_in == 0)
		{
			//ORANGE;
			//NYELLOW;
			i = 0;
			break;
		}
		else
		{
			//NORANGE;
			//YELLOW;
		}

		i++;
	}
	set_sda_outputmode();
	// or timeout
	if (i)
		return (2);

	msb = s_read_byte(ACK);		   //read the first byte (MSB)
	lsb = s_read_byte(ACK);		   //read the second byte (LSB)
	checksum = s_read_byte(noACK); //read checksum (8-bit)

	*p_value = (msb << 8) | (lsb);

	return (0);
}

float calc_sth11_temp(unsigned int t)
{
	float t_out;
	t_out = t * 0.01 - 40;

	return t_out;
}

float calc_sth11_humi(unsigned int h, int t)
{
	float rh_lin;  // rh_lin:  Humidity linear
	float rh_true; // rh_true: Temperature compensated humidity
	float t_C;	 // t_C   :  Temperature [�C]

	t_C = t * 0.01 - 40;								 //calc. temperature from ticks to [�C]
	rh_lin = C3 * h * h + C2 * h + C1;					 //calc. humidity from ticks to [%RH]
	rh_true = (t_C - 25) * (Temp1 + Temp2 * h) + rh_lin; //calc. temperature compensated humidity

	// now calc. Temperature compensated humidity [%RH]
	// the correct formula is:
	// rh_true=(t/10-25)*(0.01+0.00008*(sensor_val))+rh;
	// sensor_val ~= rh*30
	// we use:
	// rh_true=(t/10-25) * 1/8;

	if (rh_true > 100)
		rh_true = 100; //cut if the value is outside of
	if (rh_true < 0.1)
		rh_true = 0.1; //the physical possible range

	return rh_true;
}

void Read_SHT11(float *fT, float *fRH, unsigned int *tnc, unsigned int *hnc)
{
	unsigned int t;
	unsigned int h;
	float value;

	ucSens_Error = 0;

	while (s_measure(&t, 0) == 1)
	{
		ORANGE;
	}

	while (s_measure(&h, 1) == 1)
	{
		ORANGE;
	}
	NORANGE;

	iSHT_Temp = (int)(calc_sth11_temp(t) * 10);
	iSHT_Humi = (int)(calc_sth11_humi(h, t) * 10);

	//iSHT_Temp = t;
	//iSHT_Humi = h;

	value = (float)iSHT_Temp;
	*fT = value / 10;
	*tnc = t;
	value = (float)iSHT_Humi;
	*fRH = value / 10;
	*hnc = h;
}

char s_read_statusreg(unsigned char *p_value)
{
	unsigned char checksum = 0;

	s_transstart(); //transmission start
	if (s_write_byte(STATUS_REG_R))
		return 1;				   //send command to sensor
	*p_value = s_read_byte(ACK);   //read status register (8-bit)
	checksum = s_read_byte(noACK); //read checksum (8-bit)

	return 0;
}
char s_write_statusreg(unsigned char value)
{
	s_transstart(); //transmission start
	if (s_write_byte(STATUS_REG_W))
		return 1; //send command to sensor
	if (s_write_byte(value))
		return 1; //send value of status register

	return 0;
}

void s_connectionreset()
{
	unsigned char i;

	SDA_pin_out_high;
	SCL_pin_low;

	for (i = 0; i < 9; i++)
	{
		SCL_pin_high;
		Delay(3);
		SCL_pin_low;
		Delay(3);
	}

	s_transstart();
}

unsigned char s_softreset(void)
{
	s_connectionreset();		  //reset communication
								  //send RESET-command to sensor
	return (s_write_byte(RESET)); //return=1 in case of no response form the sensor
}
