#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

int msecFlag;
int secFlag;

#ifndef SIMULATOR
#include "stm32f7xx_hal.h"
#endif

unsigned char FLAG_RxCplt;
unsigned char rxBuf[20];

extern UART_HandleTypeDef huart6;

Model::Model() : modelListener(0)
{

}

#ifndef SIMULATOR

void Model::tick()
{
	// Make Screen Model Tick
	static int msTick;
	static int secTick;

	static int rxTick;

	HAL_UART_Receive_IT(&huart6, (uint8_t *)&rxBuf, 20);

	if(++msTick >= 6)
	{
		// about 100ms
		msTick = 0;
		msecFlag = 1;
	}


	if(++rxTick >= 12)
	{
		rxTick = 0;

	}

	if(++secTick >= 60)
	{
		secTick = 0;
		secFlag = 1;
	}
}

/*
 @function: HAL_UART_RxCpltCallback
 @param: (*huart_t) is UART handle Structure definition
 @brief: if Controller Receive Data to access RxFlag
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart_t)
{
	if(huart_t->Instance == USART6)
	{
		FLAG_RxCplt = 1;
	}
}

#endif
