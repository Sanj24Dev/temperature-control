#include "main.h"

GPIO_InitTypeDef GPIO_SDcard;
SPI_InitTypeDef SPI_SDcard;

/**
    Function configures the GPIO pins and SPI for SD card
    PB9: CS
    PB10: SCK
    PB14: MISO
    PB15: MOSI
*/

void sd_card_config()
{
    // configuring GPIO pins for MOSI, MISO, CS, SCK 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_SDcard.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15; // CS | SCK  |  MISO | MOSI
    GPIO_SDcard.GPIO_Mode = GPIO_Mode_AF;
    GPIO_SDcard.GPIO_OType = GPIO_OType_PP;
    GPIO_SDcard.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_SDcard.GPIO_Speed = GPIO_Speed_2MHz;
    
    GPIO_Init(GPIOA, &GPIO_SDcard);
    
    // configuring SPI
    RCC_AHB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    SPI_SDcard.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_SDcard.SPI_Mode = SPI_Mode_Master;
    SPI_SDcard.SPI_DataSize = SPI_DataSize_8b;
    SPI_SDcard.SPI_CPOL = SPI_CPOL_High;
    SPI_SDcard.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_SDcard.SPI_NSS = SPI_NSS_Hard;
    SPI_SDcard.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_Init(SPI2,&SPI_SDcard);
}

void save(tempStruct *TEMP)
{
  // save the values into the SD card
}