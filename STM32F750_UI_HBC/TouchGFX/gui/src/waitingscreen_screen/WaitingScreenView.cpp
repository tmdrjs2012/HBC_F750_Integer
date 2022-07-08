/*
 @file: WaitingScreenView.cpp
 @brief: WaitingSreen of Temperature Display
 @date: 2022-06-30
 @ref: Model.cpp, WatingScreenViewBase.cpp
 */

#include <gui/waitingscreen_screen/WaitingScreenView.hpp>

#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

#include <string.h>

#ifndef SIMULATOR
#include "stm32f7xx_hal.h"
#endif

extern unsigned char rxBuf[20];				// read data 20byte (temperature controller -> display)
unsigned char txBuf[12]	 = {0, };			// write data 12byte (display -> temperature controller -> display)

extern unsigned char FLAG_RxCplt;
unsigned char FLAG_TxCplt;
unsigned char Send_reQuestFlag;

unsigned char reQuestFlag = 1;
unsigned char TransmitFlag;

// load tick
extern int msecFlag;
extern int secFlag;

int msecCnt;

extern UART_HandleTypeDef huart6;

WaitingScreenView::WaitingScreenView()
{

}
#ifndef SIMULATOR
void WaitingScreenView::setupScreen()
{
	// Rx interrupt Enable
    WaitingScreenViewBase::setupScreen();
    HAL_UART_Receive_IT(&huart6, (uint8_t *)&rxBuf, 20);
}

void WaitingScreenView::tearDownScreen()
{
    WaitingScreenViewBase::tearDownScreen();
}



void WaitingScreenView::handleTickEvent()
{

	if(FLAG_RxCplt == 1 )
	{
		HAL_UART_Receive_IT(&huart6, (uint8_t *)&rxBuf, 20);

		FLAG_RxCplt = 0;
	}

	// 1.2 verify connection request and send feedback step
	if(rxBuf[0] == 0xfb && rxBuf[1] == 0x80 && rxBuf[19] == 0xa5)
	{

		// Controller verified connection and Display received Feedback
		reQuestFlag = 0;

		txBuf[0] = 0xfc;

		txBuf[1] = 0x90;		// 전송 시작

		txBuf[2] = 0x00;
		txBuf[3] = 0x00;
		txBuf[4] = 0x00;
		txBuf[5] = 0x00;

		txBuf[6] = 0x00;
		txBuf[7] = 0x00;
		txBuf[8] = 0x00;
		txBuf[9] = 0x00;
		txBuf[10] = 0x00;

		txBuf[11] = 0xa5;

		// Send Feedback to Controller and Change MainScreen
		HAL_NVIC_DisableIRQ(USART6_IRQn);
		HAL_UART_Transmit_IT(&huart6, (unsigned char *) &txBuf, 12);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
		reQuestFlag = 1;
		application().gotoMainScreenScreenSlideTransitionEast();
	}
	if(msecFlag)
	{
		// initialize
		msecFlag = 0;
		msecCnt++;
	}
	if(msecCnt>= 4)
	{
		// one point dot
		oneCommaText.setAlpha(255);
		oneCommaText.invalidate();

	}
	if(msecCnt >= 8)
	{
		// two point dot
		twoCommaText.setAlpha(255);
		twoCommaText.invalidate();
	}
	if(msecCnt >= 12)
	{
		// three point dot
		ThreeCommaText.setAlpha(255);
		ThreeCommaText.invalidate();
	}
	if(msecCnt >= 16)
	{

		oneCommaText.setAlpha(0);
		twoCommaText.setAlpha(0);
		ThreeCommaText.setAlpha(0);
		oneCommaText.invalidate();
		twoCommaText.invalidate();
		ThreeCommaText.invalidate();
		Send_reQuestFlag = 1;
		msecCnt = 0;
	}

	// Send Connect Request
	if(reQuestFlag == 1 && Send_reQuestFlag == 1)
	{
		// request connecting
		txBuf[0] = 0xfc;

		txBuf[1] = 0x70;

		txBuf[2] = 0x00;
		txBuf[3] = 0x00;
		txBuf[4] = 0x00;
		txBuf[5] = 0x00;

		txBuf[6] = 0x00;
		txBuf[7] = 0x00;
		txBuf[8] = 0x00;
		txBuf[9] = 0x00;
		txBuf[10] = 0x00;

		txBuf[11] = 0xa5;

		HAL_NVIC_DisableIRQ(USART6_IRQn);
		HAL_UART_Transmit(&huart6, (unsigned char *) &txBuf, 12, 100);
		HAL_NVIC_EnableIRQ(USART6_IRQn);

		Send_reQuestFlag = 0;
	}
}


#endif
