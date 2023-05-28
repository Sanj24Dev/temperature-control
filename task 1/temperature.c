#include "main.h"

uint16_t vout;
float R_NTC, temp;

GPIO_InitTypeDef GPIO_InitStruct;
ADC_InitTypeDef ADC_InitStruct;

float max_temp = 303.15, min_temp = 288.15;


/** 
    Function to configure ADC
    ADC1 Ch9 is configured to take input from pin PB1
    12 bit resolution
    84 cycles sample time
*/
void adc_config()
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    // Initialize pin PB1 for ADC input
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Initialize ADC1
    // Enable clock for ADC1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_StructInit(&ADC_InitStruct);
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;                // 12 bit resolution
    ADC_InitStruct.ADC_ExternalTrigConv = DISABLE;
    ADC_Init(ADC1, &ADC_InitStruct);
    ADC_Cmd(ADC1, ENABLE);

    // Select input channel 9 for ADC1
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_84Cycles);          // 84 cycles sample time
}

/** 
    Function to get the temperature
    Get the value from ADC and convert it to temperature
    returns temp (float)
*/
float get_temperature()
{
    // Start ADC conversion
    ADC_SoftwareStartConv(ADC1);
    // Wait until conversion is finish
    while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));

    // converting voltage to temperature
    vout = ADC_GetConversionValue(ADC1);
    R_NTC = (vout * R_100k) /(3.3 - vout);
    temp = (K25C*B)/(K25C*log(R_NTC/R_100k)+B); //Temperature in Kelvin
    return temp;
}

/** 
    Function to check if the temperature is within the range
    Get the value from ADC and convert it to temperature
    Saves the temp in the struct
    Args: tempStruct*
*/
void check_temperature(tempStruct *TEMP)
{
    // get the temperature
    TEMP->temp = get_temperature();
    TEMP->diff = 0;
    // check if the temperature
    if(temp>=min_temp && temp<=max_temp)
      TEMP->flag = SAFE;
    else
      TEMP->flag = UNSAFE;
    // record the temp in sd card
    save(TEMP);
}

/** 
    Function to check the action required - cooling or heating
    Args: tempStruct*
    returns int - COOL if the system is to be cooled
                - HEAT if the system is to be heated
*/
int check_device(tempStruct *TEMP)
{
  if(TEMP->temp > max_temp){
    TEMP->diff = max_temp - temp;
    return COOL;
  }
  else if(TEMP->temp < min_temp){
    TEMP->diff = temp - min_temp;
    return HEAT;
  }
}



