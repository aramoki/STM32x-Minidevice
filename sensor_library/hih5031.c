
#include "hih5031.h"

ADC_InitTypeDef ADC_InitStructure; //Structure for adc confguration
GPIO_InitTypeDef GPIO_InitStructure; //Structure for analog input pin
 
int msTicks;



void Delay_adc (int dly) {                                              
  int curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dly);
}


void init_ADC (void) {

	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE; // Single Channel
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // Scan on Demand
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);
  ADC_Cmd(ADC1, ENABLE);
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);

//  //SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
//  //SysTick_Config(SystemCoreClock / 1000);

}

void Read_HIH5031(float *HIH5031_humm , float temp){
	float dum1 , dum2;
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
				dum1=(ADC_GetConversionValue(ADC1)*(float)(3.0/4096));
				dum2 = ((dum1 / 3) - 0.1515) /(0.00636);
				*HIH5031_humm = (dum2 ) / (1.0546 - 0.00216 * temp);

}
