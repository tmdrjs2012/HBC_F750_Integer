/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/

/*
 @file: MainScreenViewBase.cpp
 @brief: MainSceen of Temperature Display
 @date: 2022-06-29
 @ref: MainScreenView.cpp
 @version: 4 channel Configuration Temperature
 */

/*********************************************************************************/
/******* IF YOU GENERATE CODE IN TOUCHGFX, THIS CODE WILL BE INITIALIZED  ********/
/****************** YOU SHOULD DEFINITELY BACK UP YOUR FILES  ********************/
/*********************************************************************************/

#include <gui_generated/mainscreen_screen/MainScreenViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

unsigned char heaterState[4] = {0, 0, 0, 0};
unsigned char OnOffTrigger[4];
unsigned char OnOffState = 0x00;
unsigned int Config_Temp[4];

unsigned char ButtonTrigger;

extern unsigned char Main_FLAG_TxCplt;

extern unsigned char ConBtnStateFlag;

#ifndef SIMULATOR
#include "stm32f7xx_hal.h"
#endif

MainScreenViewBase::MainScreenViewBase() :
    buttonCallback(this, &MainScreenViewBase::buttonCallbackHandler),
    flexButtonCallback(this, &MainScreenViewBase::flexButtonCallbackHandler)
{

    __background.setPosition(0, 0, 480, 272);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));

    box1.setPosition(0, 0, 480, 272);
    box1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));

    image1.setXY(14, 9);
    image1.setBitmap(touchgfx::Bitmap(BITMAP_MATRIX_BOX213_ID));

    ArmButton.forceState(true);
    ArmButton.setBoxWithBorderPosition(0, 0, 47, 150);
    ArmButton.setBorderSize(0);
    ArmButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(222, 0, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(222, 0, 0));
    ArmButton.setPosition(28, 32, 47, 150);
    ArmButton.setAction(flexButtonCallback);

    BackButton.forceState(true);
    BackButton.setBoxWithBorderPosition(0, 0, 66, 66);
    BackButton.setBorderSize(0);
    BackButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(222, 0, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(222, 0, 0));
    BackButton.setPosition(87, 116, 66, 66);
    BackButton.setAction(flexButtonCallback);

    ArmButton_1.forceState(true);
    ArmButton_1.setBoxWithBorderPosition(0, 0, 48, 150);
    ArmButton_1.setBorderSize(0);
    ArmButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(222, 0, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(222, 0, 0));
    ArmButton_1.setPosition(168, 32, 48, 150);
    ArmButton_1.setAction(flexButtonCallback);

    LegButton.forceState(true);
    LegButton.setBoxWithBorderPosition(0, 0, 66, 45);
    LegButton.setBorderSize(0);
    LegButton.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(222, 0, 0), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(222, 0, 0));
    LegButton.setPosition(87, 201, 66, 45);
    LegButton.setAction(flexButtonCallback);

    BackButton_1.forceState(true);
    BackButton_1.setBoxWithBorderPosition(0, 0, 66, 66);
    BackButton_1.setBorderSize(0);
    BackButton_1.setBoxWithBorderColors(touchgfx::Color::getColorFromRGB(138, 138, 138), touchgfx::Color::getColorFromRGB(230, 173, 16), touchgfx::Color::getColorFromRGB(0, 51, 102), touchgfx::Color::getColorFromRGB(222, 0, 0));
    BackButton_1.setPosition(87, 32, 66, 66);
    BackButton_1.setAction(flexButtonCallback);

    textArea1.setXY(106, 142);
    textArea1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1.setLinespacing(0);
    Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID39).getText());
    textArea1.setWildcard(textArea1Buffer);
    textArea1.resizeToCurrentText();
    textArea1.setTypedText(touchgfx::TypedText(T_SINGLEUSEID5));

    textArea1_1.setXY(37, 98);
    textArea1_1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1_1.setLinespacing(0);
    Unicode::snprintf(textArea1_1Buffer, TEXTAREA1_1_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID32).getText());
    textArea1_1.setWildcard(textArea1_1Buffer);
    textArea1_1.resizeToCurrentText();
    textArea1_1.setTypedText(touchgfx::TypedText(T_SINGLEUSEID6));

    textArea1_1_1.setXY(106, 216);
    textArea1_1_1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1_1_1.setLinespacing(0);
    Unicode::snprintf(textArea1_1_1Buffer, TEXTAREA1_1_1_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID38).getText());
    textArea1_1_1.setWildcard(textArea1_1_1Buffer);
    textArea1_1_1.resizeToCurrentText();
    textArea1_1_1.setTypedText(touchgfx::TypedText(T_SINGLEUSEID7));

    textArea1_1_2.setXY(106, 58);
    textArea1_1_2.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1_1_2.setLinespacing(0);
    Unicode::snprintf(textArea1_1_2Buffer, TEXTAREA1_1_2_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID14).getText());
    textArea1_1_2.setWildcard(textArea1_1_2Buffer);
    textArea1_1_2.resizeToCurrentText();
    textArea1_1_2.setTypedText(touchgfx::TypedText(T_SINGLEUSEID8));

    textArea1_1_3.setXY(178, 98);
    textArea1_1_3.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1_1_3.setLinespacing(0);
    Unicode::snprintf(textArea1_1_3Buffer, TEXTAREA1_1_3_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID15).getText());
    textArea1_1_3.setWildcard(textArea1_1_3Buffer);
    textArea1_1_3.resizeToCurrentText();
    textArea1_1_3.setTypedText(touchgfx::TypedText(T_SINGLEUSEID9));

    upCountBtn.setXY(380, 193);
    upCountBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_UP_ARROW_32_ID));
    upCountBtn.setIconXY(15, 22);
    upCountBtn.setAction(buttonCallback);

    downCountBtn.setXY(260, 193);
    downCountBtn.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_32_ID), touchgfx::Bitmap(BITMAP_BLUE_ICONS_DOWN_ARROW_32_ID));
    downCountBtn.setIconXY(15, 22);
    downCountBtn.setAction(buttonCallback);

    ArmConButton.setXY(230, 47);
    ArmConButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    ArmConButton.setLabelText(touchgfx::TypedText(T_SINGLEUSEID19));
    ArmConButton.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    ArmConButton.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    ArmConButton.setAction(buttonCallback);

    BackConButton.setXY(290, 47);
    BackConButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    BackConButton.setLabelText(touchgfx::TypedText(T_SINGLEUSEID20));
    BackConButton.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    BackConButton.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    BackConButton.setAction(buttonCallback);

    LegConButton.setXY(350, 47);
    LegConButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    LegConButton.setLabelText(touchgfx::TypedText(T_SINGLEUSEID21));
    LegConButton.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    LegConButton.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    LegConButton.setAction(buttonCallback);

    RollerConButton.setXY(410, 47);
    RollerConButton.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_ICON_BUTTON_PRESSED_ID));
    RollerConButton.setLabelText(touchgfx::TypedText(T_SINGLEUSEID22));
    RollerConButton.setLabelColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    RollerConButton.setLabelColorPressed(touchgfx::Color::getColorFromRGB(255, 255, 255));
    RollerConButton.setAction(buttonCallback);

    boxWithBorder1.setPosition(236, 120, 49, 59);
    boxWithBorder1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    boxWithBorder1.setBorderColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    boxWithBorder1.setBorderSize(2);

    boxWithBorder1_1.setPosition(296, 120, 49, 59);
    boxWithBorder1_1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    boxWithBorder1_1.setBorderColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    boxWithBorder1_1.setBorderSize(2);

    boxWithBorder1_1_1.setPosition(356, 120, 49, 59);
    boxWithBorder1_1_1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    boxWithBorder1_1_1.setBorderColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    boxWithBorder1_1_1.setBorderSize(2);

    boxWithBorder1_1_1_1.setPosition(416, 120, 49, 59);
    boxWithBorder1_1_1_1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    boxWithBorder1_1_1_1.setBorderColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    boxWithBorder1_1_1_1.setBorderSize(2);

    textArea3.setXY(306, 9);
    textArea3.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea3.setLinespacing(0);
    textArea3.setTypedText(touchgfx::TypedText(T_SINGLEUSEID25));

    ArmConText.setXY(246, 135);
    ArmConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    ArmConText.setLinespacing(0);
    Unicode::snprintf(ArmConTextBuffer, ARMCONTEXT_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID24).getText());
    ArmConText.setWildcard(ArmConTextBuffer);
    ArmConText.resizeToCurrentText();
    ArmConText.setTypedText(touchgfx::TypedText(T_SINGLEUSEID23));

    BackConText.setXY(306, 135);
    BackConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    BackConText.setLinespacing(0);
    Unicode::snprintf(BackConTextBuffer, BACKCONTEXT_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID27).getText());
    BackConText.setWildcard(BackConTextBuffer);
    BackConText.resizeToCurrentText();
    BackConText.setTypedText(touchgfx::TypedText(T_SINGLEUSEID26));

    LegConText.setXY(366, 135);
    LegConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    LegConText.setLinespacing(0);
    Unicode::snprintf(LegConTextBuffer, LEGCONTEXT_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID29).getText());
    LegConText.setWildcard(LegConTextBuffer);
    LegConText.resizeToCurrentText();
    LegConText.setTypedText(touchgfx::TypedText(T_SINGLEUSEID28));

    RollerConText.setXY(426, 135);
    RollerConText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    RollerConText.setLinespacing(0);
    Unicode::snprintf(RollerConTextBuffer, ROLLERCONTEXT_SIZE, "%s", touchgfx::TypedText(T_SINGLEUSEID31).getText());
    RollerConText.setWildcard(RollerConTextBuffer);
    RollerConText.resizeToCurrentText();
    RollerConText.setTypedText(touchgfx::TypedText(T_SINGLEUSEID30));

    add(__background);
    add(box1);
    add(image1);
    add(ArmButton);
    add(BackButton);
    add(ArmButton_1);
    add(LegButton);
    add(BackButton_1);
    add(textArea1);
    add(textArea1_1);
    add(textArea1_1_1);
    add(textArea1_1_2);
    add(textArea1_1_3);
    add(upCountBtn);
    add(downCountBtn);
    add(ArmConButton);
    add(BackConButton);
    add(LegConButton);
    add(RollerConButton);
    add(boxWithBorder1);
    add(boxWithBorder1_1);
    add(boxWithBorder1_1_1);
    add(boxWithBorder1_1_1_1);
    add(textArea3);
    add(ArmConText);
    add(BackConText);
    add(LegConText);
    add(RollerConText);
}

void MainScreenViewBase::setupScreen()
{

}

//Called when the screen transition ends
void MainScreenViewBase::afterTransition()
{
    //Interaction7
    //When screen transition ends change screen to WaitingScreen
    //Go to WaitingScreen with screen transition towards East

    //application().gotoWaitingScreenScreenSlideTransitionEast();
}

void MainScreenViewBase::buttonCallbackHandler(const touchgfx::AbstractButton& src)
{
    if (&src == &upCountBtn)
    {
        //Interaction8
        //When upCountBtn clicked execute C++ code
        //Execute C++ code

        // upButtonClicked
    	HAL_NVIC_DisableIRQ(USART6_IRQn);

		if(ConBtnStateFlag == 0x08)
		{
			++Config_Temp[0];
		}
		else if(ConBtnStateFlag == 0x04)
		{
			++Config_Temp[1];
		}
		else if(ConBtnStateFlag == 0x02)
		{
			++Config_Temp[2];
		}
		else if(ConBtnStateFlag == 0x01)
		{
			++Config_Temp[3];
		}

		ButtonTrigger = 1;
		HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
    else if (&src == &downCountBtn)
    {
        //Interaction9
        //When downCountBtn clicked execute C++ code
        //Execute C++ code

        // downButtonClicked
    	HAL_NVIC_DisableIRQ(USART6_IRQn);

		if(ConBtnStateFlag == 0x08)
		{
			--Config_Temp[0];
		}
		else if(ConBtnStateFlag == 0x04)
		{
			--Config_Temp[1];
		}
		else if(ConBtnStateFlag == 0x02)
		{
			--Config_Temp[2];
		}
		else if(ConBtnStateFlag == 0x01)
		{
			--Config_Temp[3];
		}

		ButtonTrigger = 1;
		HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
    else if (&src == &ArmConButton)
    {
        //Interaction10
        //When ArmConButton clicked call virtual function

        //Call ArmConBtnClicked
    	HAL_NVIC_DisableIRQ(USART6_IRQn);
		ArmConBtnClicked();
		HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
    else if (&src == &BackConButton)
    {
        //Interaction11
        //When BackConButton clicked call virtual function

        //Call BackConBtnClicked
    	HAL_NVIC_DisableIRQ(USART6_IRQn);
		BackConBtnClicked();
		HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
    else if (&src == &LegConButton)
    {
        //Interaction12
        //When LegConButton clicked call virtual function

        //Call LegConBtnClicked
    	HAL_NVIC_DisableIRQ(USART6_IRQn);
		LegConBtnClicked();
		HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
    else if (&src == &RollerConButton)
    {
        //Interaction13
        //When RollerConButton clicked call virtual function

        //Call RollerConBtnClicked
    	HAL_NVIC_DisableIRQ(USART6_IRQn);
		RollerConBtnClicked();
		HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
}

void MainScreenViewBase::flexButtonCallbackHandler(const touchgfx::AbstractButtonContainer& src)
{
    if (&src == &ArmButton)
    {
        //Interaction1
        //When ArmButton clicked execute C++ code
        //Execute C++ code

        // button clicked
    	HAL_NVIC_DisableIRQ(USART6_IRQn);

		OnOffState ^= 0x80;
		OnOffTrigger[0] = 1;

		HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
    else if (&src == &BackButton)
    {
        //Interaction4
        //When BackButton clicked execute C++ code
        //Execute C++ code

        // button clicked
    	HAL_NVIC_DisableIRQ(USART6_IRQn);

		OnOffState ^= 0x40;
		OnOffTrigger[1] = 1;

		HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
    else if (&src == &ArmButton_1)
    {
        //Interaction2
        //When ArmButton_1 clicked execute C++ code
        //Execute C++ code

        // button clicked
    	HAL_NVIC_DisableIRQ(USART6_IRQn);

		OnOffState ^= 0x80;
		OnOffTrigger[0] = 1;

		HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
    else if (&src == &LegButton)
    {
        //Interaction3
        //When LegButton clicked execute C++ code
        //Execute C++ code

        // button clicked
    	HAL_NVIC_DisableIRQ(USART6_IRQn);

		OnOffState ^= 0x20;
		OnOffTrigger[2] = 1;

		HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
    else if (&src == &BackButton_1)
    {
        //Interaction5
        //When BackButton_1 clicked execute C++ code
        //Execute C++ code

        // button clicked
    	HAL_NVIC_DisableIRQ(USART6_IRQn);

		OnOffState ^= 0x10;
		OnOffTrigger[3] = 1;

		HAL_NVIC_EnableIRQ(USART6_IRQn);
    }
}
