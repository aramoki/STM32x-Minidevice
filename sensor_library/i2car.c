#include "i2car.h"
//uint8_t value;



void Delayi2c(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

void init_I2C1(void){
		GPIO_InitTypeDef GPIO_InitStruct; 
		GPIO_InitTypeDef GPIO_Output;
		I2C_InitTypeDef I2C_InitStruct;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9; // Pins 6 (I2C1_SCL) and 9 (I2C1_SDA) are used
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;		// this defines the IO speed and has nothing to do with the baudrate!
		GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// this defines the output type as open drain mode (as opposed to push pull)
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
		GPIO_Init(GPIOB, &GPIO_InitStruct);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers
	
		

		GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1); //
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
	

		I2C_DeInit(I2C1);
		I2C_Cmd(I2C1, ENABLE);
		I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
		I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
		I2C_InitStruct.I2C_OwnAddress1 = 0xEE; // or 0x00 
		I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
		I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		I2C_InitStruct.I2C_ClockSpeed = 30000;
		I2C_Init(I2C1, &I2C_InitStruct);
}

uint8_t Write_24Cxx(uint16_t Addr, uint8_t Data, uint8_t Mem_Type){
	
		uint32_t timeout = I2C_TIMEOUT_MAX;
		uint8_t upper_addr,lower_addr;
		lower_addr = (uint8_t)((0x00FF)&Addr);
	
		if(Mem_Type==M24512){
			Addr = Addr>>8;
			upper_addr = (uint8_t)((0x00FF)&Addr);
		}

		I2C_GenerateSTART(I2C1, ENABLE);


		timeout = I2C_TIMEOUT_MAX; 
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
		if ((timeout--) == 0) return 0xFF;	
		}
   
		I2C_Send7bitAddress(I2C1, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);
 
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
			if ((timeout--) == 0) return 0xFF;
		}
 
		if(Mem_Type==M24512){
			I2C_SendData(I2C1, upper_addr);
			timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
			while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
				if ((timeout--) == 0) return 0xFF;
			}
		}	
		I2C_SendData(I2C1, lower_addr);

		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
			if ((timeout--) == 0) return 0xFF;
		}
  
		I2C_SendData(I2C1, Data);

		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
			if ((timeout--) == 0) return 0xFF;
		}  
 
		I2C_GenerateSTOP(I2C1, ENABLE);
  
		return 0;
}

uint8_t Read_24Cxx(uint16_t Addr, uint8_t Mem_Type){
	
		uint32_t timeout = I2C_TIMEOUT_MAX;
		uint8_t Data = 0;
		uint8_t upper_addr,lower_addr;
	
		lower_addr = (uint8_t)((0x00FF)&Addr);
	
		if(Mem_Type==M24512){
			Addr = Addr>>8;
			upper_addr = (uint8_t)((0x00FF)&Addr);
		}
		
  I2C_GenerateSTART(I2C1, ENABLE);

  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
		if ((timeout--) == 0) return 0xFF;
  } 
  
  I2C_Send7bitAddress(I2C1, MEM_DEVICE_WRITE_ADDR, I2C_Direction_Transmitter);
 
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		if ((timeout--) == 0) return 0xFF;
  } 

	if(Mem_Type==M24512){
		I2C_SendData(I2C1,upper_addr);

		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
			if ((timeout--) == 0) return 0xFF;
		}
	}		
	
	  /* Send I2C1 location address LSB */
  I2C_SendData(I2C1, lower_addr);

  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    /* If the timeout Delayi2c is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  } 
  
  //I2C1->SR1 |= (uint16_t)0x0400;

  I2C_GenerateSTART(I2C1, ENABLE);
  
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
		if ((timeout--) == 0) return 0xFf;
  } 
  
  I2C_Send7bitAddress(I2C1, MEM_DEVICE_READ_ADDR, I2C_Direction_Receiver);
   
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
		if ((timeout--) == 0) return 0xF7;
  }  
 
  I2C_AcknowledgeConfig(I2C1, DISABLE);  

  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)){
    if ((timeout--) == 0) return 0xFF;
  }   

  I2C_GenerateSTOP(I2C1, ENABLE);

  Data = I2C_ReceiveData(I2C1);
  return Data;
}


uint8_t Write_CC2A(uint16_t Addr, uint8_t Data){
	
		uint32_t timeout = I2C_TIMEOUT_MAX;
		uint8_t upper_addr,lower_addr;
		lower_addr = (uint8_t)((0x00FF)&Addr);
	
	
		I2C_GenerateSTART(I2C1, ENABLE);


		timeout = I2C_TIMEOUT_MAX; 
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
		if ((timeout--) == 0) return 0xF1;	
		}
   
		I2C_Send7bitAddress(I2C1, CARCAR_ADDR, I2C_Direction_Transmitter);
 
		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
			if ((timeout--) == 0) return 0xF2;
		}
 
	
		I2C_SendData(I2C1, lower_addr);

		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
			if ((timeout--) == 0) return 0xF3;
		}
  
		I2C_SendData(I2C1, Data);

		timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)){
			if ((timeout--) == 0) return 0xF4;
		}  
 
		I2C_GenerateSTOP(I2C1, ENABLE);
  
		return 0x00;
}

uint8_t Read_CC2A(uint16_t Addr){
	
		uint32_t timeout = I2C_TIMEOUT_MAX;
		uint8_t Data = 0;
		uint8_t lower_addr;
	
		lower_addr = (uint8_t)((0x00FF)&Addr);
	

		
  I2C_GenerateSTART(I2C1, ENABLE);

  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
		if ((timeout--) == 0) return 0xFF;
  } 
  
  I2C_Send7bitAddress(I2C1, CARCAR_ADDR, I2C_Direction_Transmitter);
 
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)){
		if ((timeout--) == 0) return 0xFF;
  } 

		
	
	  /* Send I2C1 location address LSB */
  I2C_SendData(I2C1, lower_addr);

  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    /* If the timeout Delayi2c is exeeded, exit with error code */
		if ((timeout--) == 0) return 0xFF;
  } 
  
  //I2C1->SR1 |= (uint16_t)0x0400;

  I2C_GenerateSTART(I2C1, ENABLE);
  
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)){
		if ((timeout--) == 0) return 0xFF;
  } 
  
  I2C_Send7bitAddress(I2C1, CARCAR_ADDR, I2C_Direction_Receiver);
   
  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)){
		if ((timeout--) == 0) return 0xFF;
  }  
 
  I2C_AcknowledgeConfig(I2C1, DISABLE);  

  timeout = I2C_TIMEOUT_MAX; /* Initialize timeout value */
  while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)){
    if ((timeout--) == 0) return 0xFF;
  }   

  I2C_GenerateSTOP(I2C1, ENABLE);

  Data = I2C_ReceiveData(I2C1);
  return Data;
}
