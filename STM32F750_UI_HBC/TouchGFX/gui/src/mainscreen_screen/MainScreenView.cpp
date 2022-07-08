/*
 @file: MainScreenView.cpp
 @brief: MainSceen of Temperature Display
 @date: 2022-06-29
 @ref: Model.cpp, MainScreenViewBase.cpp
 @version: 4 channel Configuration Temperature
 */

//--------------------------//
//	D0(PC7) is USART6_RX	//
//	D1(PC6) is USART6_TX	//
//--------------------------//

#include <gui/mainscreen_screen/MainScreenView.hpp>

#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

#include <string.h>

#ifndef SIMULATOR
#include "stm32f7xx_hal.h"
#endif

extern unsigned char rxBuf[20];					// read data 20byte
unsigned char Main_txBuf[12]	 = {0, };		// write data 12byte
extern unsigned char FLAG_RxCplt;
unsigned char Main_FLAG_TxCplt;
float ReadTemp[4];								// arm, back, leg, roller read temperature variable

int ConfigTemp[4] = {35, 35, 35, 35};			// Configuration Temperature 4 channels
extern unsigned char OnOffState;
extern unsigned char heaterState[4];
extern unsigned char OnOffTrigger[4];
extern unsigned char ButtonTrigger;
extern unsigned int Config_Temp[4];

// 2byte data type
unsigned int targetIntVal_ch1;
unsigned int targetIntVal_ch2;
unsigned int targetIntVal_ch3;
unsigned int targetIntVal_ch4;

unsigned char toggleTestCode;
unsigned int Init_Temp = 35;
unsigned int ConfigMyTemp[0];

unsigned char PastOnOffState;
extern UART_HandleTypeDef huart6;

unsigned char toggleTime;
unsigned char ReadOnOffState;

unsigned char getToggle;

unsigned char ToggleValue;

int displayContinous[4];
unsigned char countCheck;
unsigned char ChangeCT = 0;
//--------------------------------------------------------------------------------------------------------

unsigned char ConBtnStateFlag;

int DisplayVal[4];
int DisplayFahri[4];
int Config_Fahri[4];

MainScreenView::MainScreenView()
{

}

#ifndef SIMULATOR

void MainScreenView::setupScreen()
{
    MainScreenViewBase::setupScreen();
    // Rx interrupt Enable
    HAL_NVIC_EnableIRQ(USART6_IRQn);
    HAL_UART_Receive_IT(&huart6, (uint8_t *)&rxBuf, 20);
}

void MainScreenView::tearDownScreen()
{
    MainScreenViewBase::tearDownScreen();
}

void MainScreenView::handleTickEvent()
{
	static unsigned int noResponseTime;
	static unsigned char display_time;

	++display_time;

	// CallBack Rx Data 20 Bytes
	if(FLAG_RxCplt == 1 )
	{
		HAL_UART_Receive_IT(&huart6, (uint8_t *)&rxBuf, 20);

		FLAG_RxCplt = 0;
	}

	// Read Temperature 4 channels
	if( (rxBuf[0] == 0xfb) && (rxBuf[1] == 0xa0) && (rxBuf[19] == 0xa5) )
	{
		//Read Temperature from Temperature Controller
		if(rxBuf[2] < 0x0A && rxBuf[6] < 0x0A && rxBuf[10] < 0x0A && rxBuf[14] < 0x0A)
		{

			//CH1 Temperature	= arm
			ReadTemp[0] = ((float)((rxBuf[2]<<8) + rxBuf[3]))/10.0;
			//CH2 Temperature	= Back
			ReadTemp[1] = ((float)((rxBuf[6]<<8) + rxBuf[7]))/10.0;
			//CH3 Temperature	= Leg
			ReadTemp[2] = ((float)((rxBuf[10]<<8) + rxBuf[11]))/10.0;
			//CH4 Temperature
			ReadTemp[3] = ((float)((rxBuf[14]<<8) + rxBuf[15]))/10.0;

			//I2C Temperature
			//tempIntVal_i2c = ((float)((rxBuf[7]<<8)+rxBuf[8]))/10.0;

			//CH1 Configuration Temperature - All Shareing
			ConfigTemp[0] = ((float)((rxBuf[4]<<8) + rxBuf[5]))/10.0;

			//CH2 Configuration Temperature
			ConfigTemp[1] = ((float)((rxBuf[8]<<8)+rxBuf[9]))/10.0;

			//CH3 Configuration Temperature
			ConfigTemp[2] = ((float)((rxBuf[12]<<8)+rxBuf[13]))/10.0;

			//CH4 Configuration Temperature
			ConfigTemp[3] = ((float)((rxBuf[16]<<8)+rxBuf[17]))/10.0;

			OnOffState = rxBuf[18];
		}
	}

	// Check Disconnect
	if(rxBuf[1] != 0xa0)
	{

		++noResponseTime;
		if(noResponseTime >= 300)
		{
			// count 300 is about 10sec
			noResponseTime = 0;
			application().gotoWaitingScreenScreenSlideTransitionEast();

		}

	}
	else
	{
		noResponseTime = 0;
	}

	rxBuf[1] = 0x00;


	if(ReadTemp[0] <= 22)
	{
		ReadTemp[0] = 0;
	}
	if(ReadTemp[1] <= 22)
	{
		ReadTemp[1] = 0;
	}
	if(ReadTemp[2] <= 22)
	{
		ReadTemp[2] = 0;
	}
	if(ReadTemp[3] <= 22)
	{
		ReadTemp[3] = 0;
	}

	if( ((OnOffState & (0x1 << 7)) >= 1) )
	{
		// Heater On and color change
		ArmButton.forceState(true);
		ArmButton_1.forceState(true);

		// Button Color Range ( Yellow, Orange, Dark Orange, Red )
		if(ReadTemp[0] <= 30)
		{
			ArmButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 255, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
			ArmButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 255, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[0] >= 30.4 && ReadTemp[0] < 40)
		{
			ArmButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 155, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
			ArmButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 155, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[0] >= 40.4 && ReadTemp[0] < 50)
		{
			ArmButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 95, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
			ArmButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 95, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[0] >= 50.4)
		{
			ArmButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 0, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
			ArmButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 0, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		ArmButton.invalidate();
		ArmButton_1.invalidate();
	}
	else
	{
		ArmButton.forceState(true);
		ArmButton_1.forceState(true);

		ArmButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		ArmButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		ArmButton.invalidate();
		ArmButton_1.invalidate();
	}

	if( ((OnOffState & (0x1 << 6)) >= 1) )
	{
		// Heater On and color change
		BackButton.forceState(true);

		// Button Color Range ( Yellow, Orange, Dark Orange, Red )
		if(ReadTemp[1] <= 30)
		{
			BackButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 255, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[1] >= 30.4 && ReadTemp[1] < 40)
		{
			BackButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 155, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[1] >= 40.4 && ReadTemp[1] < 50)
		{
			BackButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 95, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[1] >= 50.4)
		{
			BackButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 0, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		BackButton.invalidate();
	}
	else
	{
		BackButton.forceState(true);

		BackButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		BackButton.invalidate();
	}

	// leg button getstate
	if( ((OnOffState & (0x1 << 5)) >= 1) )
	{
		// Heater On and color change
		LegButton.forceState(true);

		// Button Color Range ( Yellow, Orange, Dark Orange, Red )
		if(ReadTemp[2] <= 30)
		{
			LegButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 255, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[2] >= 30.4 && ReadTemp[2] < 40)
		{
			LegButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 155, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[2] >= 40.4 && ReadTemp[2] < 50)
		{
			LegButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 95, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[2] >= 50.4)
		{
			LegButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 0, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		LegButton.invalidate();
	}
	else
	{
		LegButton.forceState(true);

		LegButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		LegButton.invalidate();
	}

	// roller button getstate
	if( ((OnOffState & (0x1 << 4)) >= 1) )
	{
		// Heater On and color change
		BackButton_1.forceState(true);

		// Button Color Range ( Yellow, Orange, Dark Orange, Red )
		if(ReadTemp[3] <= 30)
		{
			BackButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 255, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[3] >= 30.4 && ReadTemp[3] < 40)
		{
			BackButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 155, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[3] >= 40.4 && ReadTemp[3] < 50)
		{
			BackButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 95, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		else if(ReadTemp[3] >= 50.4)
		{
			BackButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(255, 0, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		}
		BackButton_1.invalidate();
	}
	else
	{
		BackButton_1.forceState(true);

		BackButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(255, 0, 0));
		BackButton_1.invalidate();
	}

	if(ConBtnStateFlag == 0x08)
	{
		// push button Arm configure

		// change text color (initialize BLACK)
		ArmConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		ArmConText.invalidate();

		BackConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		BackConText.invalidate();

		LegConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		LegConText.invalidate();

		RollerConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		RollerConText.invalidate();

		ArmConText.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
		ArmConText.invalidate();

		countCheck--;

		if(ButtonTrigger == 1)
		{
			countCheck = 150;

			ConfigMyTemp[0] = Init_Temp + Config_Temp[0];
			ConfigTemp[0] = ConfigMyTemp[0];
			if(ConfigMyTemp[0] < 35)
			{
				ConfigMyTemp[0] = 35;
				ConfigTemp[0] = ConfigMyTemp[0];
				Config_Temp[0] = 0;

			}
			else if(ConfigMyTemp[0] > 65)
			{
				ConfigMyTemp[0] = 65;
				ConfigTemp[0] = ConfigMyTemp[0];
				Config_Temp[0] = 30;
			}

			ButtonTrigger = 0;
			Main_FLAG_TxCplt = 1;
		}
	}
	else if(ConBtnStateFlag == 0x04)
	{
		// push button Back configure

		// change text color
		ArmConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		ArmConText.invalidate();

		BackConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		BackConText.invalidate();

		LegConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		LegConText.invalidate();

		RollerConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		RollerConText.invalidate();


		BackConText.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
		BackConText.invalidate();

		countCheck--;

		if(ButtonTrigger == 1)
		{
			countCheck = 150;

			ConfigMyTemp[1] = Init_Temp + Config_Temp[1];
			ConfigTemp[1] = ConfigMyTemp[1];
			if(ConfigMyTemp[1] < 35)
			{
				ConfigMyTemp[1] = 35;
				ConfigTemp[1] = ConfigMyTemp[1];
				Config_Temp[1] = 0;

			}
			else if(ConfigMyTemp[1] > 65)
			{
				ConfigMyTemp[1] = 65;
				ConfigTemp[1] = ConfigMyTemp[1];
				Config_Temp[1] = 30;
			}

			ButtonTrigger = 0;
			Main_FLAG_TxCplt = 1;
		}
	}
	else if(ConBtnStateFlag == 0x02)
	{
		// push button Lag configure

		// change text color
		ArmConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		ArmConText.invalidate();

		BackConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		BackConText.invalidate();

		LegConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		LegConText.invalidate();

		RollerConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		RollerConText.invalidate();

		LegConText.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
		LegConText.invalidate();

		countCheck--;

		if(ButtonTrigger == 1)
		{
			countCheck = 150;

			ConfigMyTemp[2] = Init_Temp + Config_Temp[2];
			ConfigTemp[2] = ConfigMyTemp[2];
			if(ConfigMyTemp[2] < 35)
			{
				ConfigMyTemp[2] = 35;
				ConfigTemp[2] = ConfigMyTemp[2];
				Config_Temp[2] = 0;

			}
			else if(ConfigMyTemp[2] > 65)
			{
				ConfigMyTemp[2] = 65;
				ConfigTemp[2] = ConfigMyTemp[2];
				Config_Temp[2] = 30;
			}

			ButtonTrigger = 0;
			Main_FLAG_TxCplt = 1;
		}
	}
	else if(ConBtnStateFlag == 0x01)
	{
		// push button Roller configure

		// change text color
		ArmConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		ArmConText.invalidate();

		BackConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		BackConText.invalidate();

		LegConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		LegConText.invalidate();

		RollerConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		RollerConText.invalidate();

		RollerConText.setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
		RollerConText.invalidate();

		countCheck--;

		if(ButtonTrigger == 1)
		{
			countCheck = 150;

			ConfigMyTemp[3] = Init_Temp + Config_Temp[3];
			ConfigTemp[3] = ConfigMyTemp[3];
			if(ConfigMyTemp[3] < 35)
			{
				ConfigMyTemp[3] = 35;
				ConfigTemp[3] = ConfigMyTemp[3];
				Config_Temp[3] = 0;

			}
			else if(ConfigMyTemp[3] > 65)
			{
				ConfigMyTemp[3] = 65;
				ConfigTemp[3] = ConfigMyTemp[3];
				Config_Temp[3] = 30;
			}

			ButtonTrigger = 0;
			Main_FLAG_TxCplt = 1;
		}
	}

	else if(ConBtnStateFlag == 0x00)
	{
		// change text color
		ArmConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		ArmConText.invalidate();

		BackConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		BackConText.invalidate();

		LegConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		LegConText.invalidate();

		RollerConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
		RollerConText.invalidate();
	}

	// if you push button after 5sec
	if(countCheck <= 0)
	{
		ConBtnStateFlag = 0x00;
	}


	if(display_time >= 30)
	{
		// about 0.5sec

		// Arm
		if( ((OnOffState & (0x1 << 7)) >= 1) )
		{
			if(ReadTemp[0] >= (ConfigTemp[0] - 0.5) && ReadTemp[0] <= (ConfigTemp[0] + 0.4))
			{
				displayContinous[0] = 1;
			}
			else if(ReadTemp[0] >= (ConfigTemp[0]+1.5) || ReadTemp[0] < (ConfigTemp[0] - 1.5) )
			{
				displayContinous[0] = 0;
			}
			if(displayContinous[0] == 0)
			{
				DisplayVal[0] = (int)ReadTemp[0];
			}
			if(displayContinous[0] == 1)
			{
				DisplayVal[0] = ConfigTemp[0];
			}
		}
		else
		{
			displayContinous[0] = 0;
			DisplayVal[0] = (int)ReadTemp[0];
		}

		// Back
		if( ((OnOffState & (0x1 << 6)) >= 1) )
		{
			if(ReadTemp[1] >= (ConfigTemp[1] - 0.5) && ReadTemp[1] <= (ConfigTemp[1] + 0.4))
			{
				displayContinous[1] = 1;
			}
			else if(ReadTemp[1] >= (ConfigTemp[1]+1.5) || ReadTemp[1] < (ConfigTemp[1] - 1.5) )
			{
				displayContinous[1] = 0;
			}
			if(displayContinous[1] == 0)
			{
				DisplayVal[1] = (int)ReadTemp[1];
			}
			if(displayContinous[1] == 1)
			{
				DisplayVal[1] = ConfigTemp[1];
			}
		}
		else
		{
			displayContinous[1] = 0;
			DisplayVal[1] = (int)ReadTemp[1];
		}

		// Lag
		if( ((OnOffState & (0x1 << 5)) >= 1) )
		{
			if(ReadTemp[2] >= (ConfigTemp[2] - 0.5) && ReadTemp[2] <= (ConfigTemp[2] + 0.4))
			{
				displayContinous[2] = 1;
			}
			else if(ReadTemp[2] >= (ConfigTemp[2]+1.5) || ReadTemp[2] < (ConfigTemp[2] - 1.5) )
			{
				displayContinous[2] = 0;
			}
			if(displayContinous[2] == 0)
			{
				DisplayVal[2] = (int)ReadTemp[2];
			}
			if(displayContinous[2] == 1)
			{
				DisplayVal[2] = ConfigTemp[2];
			}
		}
		else
		{
			displayContinous[2] = 0;
			DisplayVal[2] = (int)ReadTemp[2];
		}

		// Roller
		if( ((OnOffState & (0x1 << 4)) >= 1) )
		{
			if(ReadTemp[3] >= (ConfigTemp[3] - 0.5) && ReadTemp[3] <= (ConfigTemp[3] + 0.4))
			{
				displayContinous[3] = 1;
			}
			else if(ReadTemp[3] >= (ConfigTemp[3]+1.5) || ReadTemp[3] < (ConfigTemp[3] - 1.5) )
			{
				displayContinous[3] = 0;
			}
			if(displayContinous[3] == 0)
			{
				DisplayVal[3] = (int)ReadTemp[3];
			}
			if(displayContinous[3] == 1)
			{
				DisplayVal[3] = ConfigTemp[3];
			}
		}
		else
		{
			displayContinous[3] = 0;
			DisplayVal[3] = (int)ReadTemp[3];
		}

		// Channel 1 Read Temp
		Unicode::snprintf(textArea1_1Buffer, 10, "%d", DisplayVal[0]);
		textArea1_1.setWildcard(textArea1_1Buffer);		// arm
		textArea1_1.invalidate();

		Unicode::snprintf(textArea1_1_3Buffer, 10, "%d", DisplayVal[0]);
		textArea1_1_3.setWildcard(textArea1_1_3Buffer);		// arm
		textArea1_1_3.invalidate();

		// Channel 2 Read Temp
		Unicode::snprintf(textArea1Buffer, 10, "%d", DisplayVal[1]);
		textArea1.setWildcard(textArea1Buffer);		// Back
		textArea1.invalidate();;

		// Channel 3 Read Temp
		Unicode::snprintf(textArea1_1_1Buffer, 10, "%d", DisplayVal[2]);
		textArea1_1_1.setWildcard(textArea1_1_1Buffer);		// Back
		textArea1_1_1.invalidate();

		// Channel 4 Read Temp
		Unicode::snprintf(textArea1_1_2Buffer, 10, "%d", DisplayVal[3]);
		textArea1_1_2.setWildcard(textArea1_1_2Buffer);		// Back
		textArea1_1_2.invalidate();

		Unicode::snprintf(ArmConTextBuffer, 10, "%d", ConfigTemp[0]);
		ArmConText.setWildcard(ArmConTextBuffer);
		ArmConText.invalidate();

		Unicode::snprintf(BackConTextBuffer, 10, "%d", ConfigTemp[1]);
		BackConText.setWildcard(BackConTextBuffer);
		BackConText.invalidate();

		Unicode::snprintf(LegConTextBuffer, 10, "%d", ConfigTemp[2]);
		LegConText.setWildcard(LegConTextBuffer);
		LegConText.invalidate();

		Unicode::snprintf(RollerConTextBuffer, 10, "%d", ConfigTemp[3]);
		RollerConText.setWildcard(RollerConTextBuffer);
		RollerConText.invalidate();

		display_time = 0;
	}


	// Button Event Trigger
	if(OnOffTrigger[0] == 1)
	{
		OnOffTrigger[0] = 0;
		Main_FLAG_TxCplt  = 1;
	}
	if(OnOffTrigger[1] == 1)
	{
		OnOffTrigger[1] = 0;
		Main_FLAG_TxCplt  = 1;
	}
	if(OnOffTrigger[2] == 1)
	{
		OnOffTrigger[2] = 0;
		Main_FLAG_TxCplt  = 1;
	}
	if(OnOffTrigger[3] == 1)
	{
		OnOffTrigger[3] = 0;
		Main_FLAG_TxCplt  = 1;
	}

	// Send Data to Temperature Controller
	if(Main_FLAG_TxCplt == 1)
	{
		targetIntVal_ch1 = ConfigTemp[0]*10;
		targetIntVal_ch2 = ConfigTemp[1]*10;
		targetIntVal_ch3 = ConfigTemp[2]*10;
		targetIntVal_ch4 = ConfigTemp[3]*10;

		// Send 12byte --- Configuration Temperature and ON/OFF
		Main_txBuf[0] = 0xfc;

		Main_txBuf[1] = 0xb0;

		Main_txBuf[2] = (targetIntVal_ch1 & (0xFF << 8)) >> 8;
		Main_txBuf[3] = (targetIntVal_ch1 & (0xFF << 0)) >> 0;

		Main_txBuf[4] = (targetIntVal_ch2 & (0xFF << 8)) >> 8;
		Main_txBuf[5] = (targetIntVal_ch2 & (0xFF << 0)) >> 0;

		Main_txBuf[6] = (targetIntVal_ch3 & (0xFF << 8)) >> 8;
		Main_txBuf[7] = (targetIntVal_ch3 & (0xFF << 0)) >> 0;

		Main_txBuf[8] = (targetIntVal_ch4 & (0xFF << 8)) >> 8;
		Main_txBuf[9] = (targetIntVal_ch4 & (0xFF << 0)) >> 0;

		Main_txBuf[10] = OnOffState;			// Button State

		Main_txBuf[11] = 0xa5;
		HAL_NVIC_DisableIRQ(USART6_IRQn);
		HAL_UART_Transmit_IT(&huart6, (unsigned char *) &Main_txBuf, 12);
		HAL_NVIC_EnableIRQ(USART6_IRQn);
		Main_FLAG_TxCplt = 0;
	}

	PastOnOffState = OnOffState;

}

// Arm Target Temperature Button Event
void MainScreenView::ArmConBtnClicked()
{
	ConBtnStateFlag = 0x00;
	HAL_Delay(10);
	countCheck = 150;
	ConBtnStateFlag = 0x08;
}

// Back Target Temperature Button Event
void MainScreenView::BackConBtnClicked()
{
	ConBtnStateFlag = 0x00;
	HAL_Delay(10);

	countCheck = 150;
	ConBtnStateFlag = 0x04;
}

// Leg Target Temperature Button Event
void MainScreenView::LegConBtnClicked()
{
	ConBtnStateFlag = 0x00;
	HAL_Delay(10);

	countCheck = 150;
	ConBtnStateFlag = 0x02;
}

// Roller Target Temperature Button Event
void MainScreenView::RollerConBtnClicked()
{
	ConBtnStateFlag = 0x00;
	HAL_Delay(10);

	countCheck = 150;
	ConBtnStateFlag = 0x01;
}

#endif

