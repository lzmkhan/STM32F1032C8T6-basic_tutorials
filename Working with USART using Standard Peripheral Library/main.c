/**
  ******************************************************************************
  * @file    main.c
  * @author  Khan M
  * @version V1.0
  * @date   16-October-2018
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stdint.h"


void initUsart(void);

void initUsart(void){

	// Configure the Pins PA2 and PA3 to enable Usart 2
	// Enable GPIOA peripheral clock.
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	// Preparing struct to configure the pins as output.
	GPIO_InitTypeDef gpioStruct1;
	GPIO_StructInit(&gpioStruct1);


	// Setting up the pin config for PIN B12, B13 as input.
	gpioStruct1.GPIO_Mode = GPIO_Mode_AF_PP;
	gpioStruct1.GPIO_Pin = GPIO_Pin_2;
	gpioStruct1.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &gpioStruct1);

	// Preparing struct to configure the pins as input.
	GPIO_InitTypeDef gpioStruct2;
	GPIO_StructInit(&gpioStruct2);


	// Setting up the pin config for PIN B12, B13 as input.
	gpioStruct2.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpioStruct2.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &gpioStruct2);


	// Enable the usart peripheral clock.
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	// Setting up USART 2
	USART_InitTypeDef usart2;

	USART_StructInit(&usart2);

	// Initialize the USART2 peripheral with above settings.
	USART_Init(USART2, &usart2);

	// Enable the usart2 peripheral
	USART_Cmd(USART2, ENABLE);

}

int main(void)
{
	// Execute the initialization logic
	initUsart();

	// Always good idea to define constant literals as macros
	#define LENGTH 128
	#define SIZE 13

	// Character array to store 127 characters that is inputed in the putty terminal
	char buffer[LENGTH];

	// declaring an index to keep track of how many characters received and what index they are stored in the buffer array
	uint16_t index = 0;
	const char buffer1[SIZE] = "FROM STM32: ";

	// Start of infinite loop
	while(1){

			// Check whether the receiver register has data.
			if(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == SET){
				// if receive Data register is not empty. Read the character and store it in buffer array
 				buffer[index] = USART_ReceiveData(USART2);

 				// Check if we have received 127 characters.
				if(index == (LENGTH -1)){

				// Since we have received 127 characters, we got no place to store any more. So reset the index to start of array
				index=0;

				// Index to keep track of characters we are going to send back to terminal
				uint8_t  x = 0,y = 0;

				// echoing back all 127 characters received so far.
				while(y<SIZE){
					if(USART_GetFlagStatus(USART2,USART_FLAG_TXE)){
											// Send a character
											USART_SendData(USART2, buffer1[y]);
											// Go to next position
											y++;
										}
				}
				while(x <= LENGTH-1){
					// Check if transmit register is empty
					if(USART_GetFlagStatus(USART2,USART_FLAG_TXE)){
						// Send a character
						USART_SendData(USART2, buffer[x]);
						// Go to next position
						x++;
					}
				}
				}else{
					// if we haven't received 127 characters, set the index to next position.
					index++;
				}
			}

}
}
