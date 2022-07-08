#ifndef MAINSCREENVIEW_HPP
#define MAINSCREENVIEW_HPP

#include <gui_generated/mainscreen_screen/MainScreenViewBase.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>

class MainScreenView : public MainScreenViewBase
{
public:
    MainScreenView();
    virtual ~MainScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
protected:
    //virtual void ToggleButtonClicked();
    virtual void ArmConBtnClicked();
    virtual void BackConBtnClicked();
    virtual void LegConBtnClicked();
    virtual void RollerConBtnClicked();
    //virtual void CtoFToggleBtnClicked();
};

#endif // MAINSCREENVIEW_HPP
