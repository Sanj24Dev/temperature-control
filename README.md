
# Task 1 #
Develop firmware for a closed loop temperature control system in a 3D Printer 


### Table of Contents: ###
- Overview
- Architecture and design
- Installation and setup
- Configuration
- Code documentation
- Usage guide
- Appendices


## Overview ##
The temperature control firmware is designed to control the printer’s extruder temperature. The system has a heating element to increase the temperature and a cooling fan to decrease the temperature. The system also has a temperature sensor to measure the temperature of the heating element.The printer’s extruder temperature is always maintained within a specified range. The system logs the temperature readings and heating/cooling cycles to an external SD card.

## Architecture and design ##
System Components:
- Microcontroller: STM32F407VET6
- Temperature sensor: 100K NTC Thermistor
- Heating element: resistive heating coil
- Cooling fan
- SD card



Fig. Block diagram



The system follows a feedback-control architecture. It continuously monitors the temperature values and adjusts based on this feedback. The ADC reads the input voltage and converts it to digital value. This voltage is used to find the temperature as follows:

Find the resistance with Vout = ADC output
Resistance = (Vout * 100000) /(Vsupply - Vout)
Using this resistance value, find the temperature in Kelvin
Temp_K = (T0*B_coeff)/(T0*log(Resistance/100000)+B_coeff)
Here, B_coeff is the beta value of the thermistor, and the value used is 3950. T0 0℃ Kelvin 

Now the system checks if the temperature is within the safe range. If not, then the duty cycle for the PWM is looked up in the following table:

Fig. Lookup table

Once the PWM is set, the temperature is checked again, and based on this feedback it will adjust the temperature accordingly.

## Installation and setup ##
The project is developed on IAR embedded workbench for ARM. The library used for the programming is the Standard Peripheral Library(SPL). Selected files from the SPL are chosen and saved in the project. So separate downloads are not necessary.
Keep 3.3V as input voltage. Refer to this diagram to setup the circuit:

Fig. Circuit

## Configurations ##
- The firmware provides the following configuration options:
- Thermistor input pin: ADC1-3, 16 channels each
- ADC resolution: 6 bits, 8 bits, 10 bits, 12 bits
- ADC sample time: 3, 15,28,56, 84, 112, 144,480 cycles
- Cooling fan PWM signal: 10 general purpose timers
- Heating element PWM signal: 10 general purpose timers
- SD card SPI pins: SPI1-3

## Code documentation ##
- main.c: This file has the implementation of temperature control. It also takes the temperature range input from the user.
- temperature.c: This file has driver functions to configure the ADC, get the thermistor values, convert it to temperature, and log the temperature reading into the SD card. These functions are called by the main.c
- controller.c: This file has the functions to control the heating element and cooling fan, and log the heating/cooling cycle into the SD card. These functions are called by the main.c when the temperature goes beyond the safe range.

Fig. Control flow

## Usage Guide ##
1. Set up the circuit as shown in Fig. circuit
2. Power on the microcontroller with the firmware flashed.

## Appendices ##
STM32F407VET6 datasheet: https://www.mouser.in/datasheet/2/389/stm32f405rg-1851084.pdf
NTC thermistor datasheet: https://www.tme.eu/Document/f9d2f5e38227fc1c7d979e546ff51768/NTCM-100K-B3950.pdf 

