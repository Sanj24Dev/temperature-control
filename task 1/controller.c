#include "main.h"

TIM_TimeBaseInitTypeDef TIM_InitStruct;
TIM_OCInitTypeDef TIM_OCStruct_cooling, TIM_OCStruct_heating;
GPIO_InitTypeDef GPIO_InitStructA;

float diff, opt_temp = 298.15;
int duty = 0;


/** 
    Function to configure timers and GPIOs for PWM output
    Timer: TIM3
    Cooling element: PA6, Ch1 in timer
    Heating element: PA7, Ch2 in timer
*/
void controllers_config()
{
    // GPIO PA6 for cooling fan, CH2 PA7 for heating element
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    
    GPIO_StructInit(&GPIO_InitStructA);
    GPIO_InitStructA.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_InitStructA.GPIO_Mode = GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
    GPIO_Init(GPIOA, &GPIO_InitStructA);
  
    // Configuring the timer TIM3
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructInit(&TIM_InitStruct);   
    TIM_InitStruct.TIM_Period = 9999; //  == 4s
    TIM_InitStruct.TIM_Prescaler = 0; // == 10,000Hz
    TIM_TimeBaseInit(TIM3, &TIM_InitStruct);
    
    // Output capture structure for cooling fan with 0 initial duty cycle
    TIM_OCStructInit(&TIM_OCStruct_cooling);
    TIM_OCStruct_cooling.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCStruct_cooling.TIM_Pulse = 0;
    TIM_OCStruct_cooling.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC1Init(TIM3, &TIM_OCStruct_cooling);
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
    
    // Output capture structure for heating element with 0 initial duty cycle
    TIM_OCStructInit(&TIM_OCStruct_heating);
    TIM_OCStruct_heating.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCStruct_heating.TIM_Pulse = 0;
    TIM_OCStruct_heating.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OC2Init(TIM3, &TIM_OCStruct_heating);
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
}

/** 
    Function to control the temperature
    Args: tempStruct *
    Adjusts the duty cycle of the heating and cooling element as required
*/
void control_temp(tempStruct *TEMP)
{  
    // checking what action to take up - heating or cooling
    if(check_device(TEMP)){
      // getting the PWM from the lookup
      duty = get_duty_cycle(TEMP->diff);
      // setting PWM for cooling fan
      TIM_OCStruct_cooling.TIM_Pulse = 10000*duty/100 - 1;
      // turning heating element OFF
      TIM_OCStruct_heating.TIM_Pulse = 0;
      TIM_OC1Init(TIM3, &TIM_OCStruct_cooling);
      TIM_OC2Init(TIM3, &TIM_OCStruct_heating);
    }
    else{
      // getting the PWM from the lookup
      duty = get_duty_cycle(TEMP->diff);
      // turning the cooling lement OFF
      TIM_OCStruct_cooling.TIM_Pulse = 0;
      // setting PWM for heating element
      TIM_OCStruct_heating.TIM_Pulse = 10000*duty/100 - 1;
      TIM_OC1Init(TIM3, &TIM_OCStruct_cooling);
      TIM_OC2Init(TIM3, &TIM_OCStruct_heating);
    }
    save(TEMP);
}

/** 
    Function to find duty cycle from the lookup
    Args: temp difference (float) = offset from the desired range
    returns the corresponding duty cycle (int)
*/
int get_duty_cycle(float diff)
{
    if (diff<=0.5)
      return 20;
    else if (diff <= 1)
      return 40;
    else if (diff <= 1.5)
      return 60;
    else if (diff <= 2)
      return 80;
    else
      return 100;
}


