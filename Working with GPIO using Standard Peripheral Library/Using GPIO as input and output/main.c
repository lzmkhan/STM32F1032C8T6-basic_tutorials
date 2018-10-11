/**
  ******************************************************************************
  * @file    main.c
  * @author  M khan
  * @version V1.0
  * @date    12-October-2018
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
			
void initializeGPIOPorts(void);

void initializeGPIOPorts(void){

	    // Enable GPIOB peripheral clock.
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

		// Enable GPIOC peripheral clock.
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

		// Preparing struct to configure the pins as input.
		GPIO_InitTypeDef gpioStruct;
		GPIO_StructInit(&gpioStruct);

		// Setting up the pin config for PIN B12, B13 as input.
		gpioStruct.GPIO_Mode = GPIO_Mode_IPD;
		gpioStruct.GPIO_Pin = (GPIO_Pin_12) | (GPIO_Pin_13);
		gpioStruct.GPIO_Speed = GPIO_Speed_2MHz;

		GPIO_Init(GPIOB, &gpioStruct);

		// Preparing struct to configure the pins as output.
		GPIO_InitTypeDef gpioStructOut;
		GPIO_StructInit(&gpioStructOut);

		// Setting up the pin config for PIN C13 as output(LED)
		// Use Push/pull configuration so that the pin value is either high or low
		gpioStructOut.GPIO_Mode = GPIO_Mode_Out_PP;
		gpioStructOut.GPIO_Pin = GPIO_Pin_13;
		gpioStructOut.GPIO_Speed = GPIO_Speed_2MHz;

		GPIO_Init(GPIOC, &gpioStructOut);

}

int main(void)
{

	// Initialize the pins to respective configuration.
	initializeGPIOPorts();

	while(1){
		if(!(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)|| GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))){
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
		}else{
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
		}
	}

}
