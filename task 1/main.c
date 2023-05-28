/**
  Firmware: Temperature control system
  By Sanjana Ganesh Nayak
  Date: 28 May, 2023


  Overview:
    The temperature control firmware is designed to control the printer’s extruder temperature. 
    The system has a heating element to increase the temperature and a cooling fan to decrease the temperature. 
    The system also has a temperature sensor to measure the temperature of the heating element.
    The printer’s extruder temperature is always maintained within a specified range. 
    The system logs the temperature readings and heating/cooling cycles to an external SD card.

  Files:
    - main.c: has the implementation of temperature control. It constantly checks the temperature and controls it
    - main.h: has function declarations and structure and constant definitions
    - temperature.c: has the driver code for ADC. It has functions to read the ADC, get temperature, check temperature and determine action to control it
    - controller.c: has the driver code for the cooling and heating element. It has functions to configure the timers, send pwm signals, and lookup for temp-duty cycle
    - sd_card.c: has the driver code for SD card

  Pins used:
    - PB1: Temperature sensor
    - PA6: Heating element
    - PA7: Cooling fan
  
*/

#include "main.h"

int flag;
tempStruct TEMP;

int main()
{
    // configuring the peripherals
    adc_config();
    sd_card_config();
    controllers_config();
    
    // main control code
    // monitors the temperature constantly and initiates control measure if the temperature goes beyond the range
    while(1){
      check_temperature(&TEMP);
      if(TEMP.flag == UNSAFE){
        control_temp(&TEMP);
      }   
    }  
}
