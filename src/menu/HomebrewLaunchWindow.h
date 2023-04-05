#ifndef _HOMEBREW_LAUNCHER_WINDOW_H_
#define _HOMEBREW_LAUNCHER_WINDOW_H_


#include <gui/GuiFrame.h>
#include <gui/GuiText.h>
#include <gui/GuiImage.h>
#include <gui/GuiButton.h>
#include <gui/GuiTrigger.h>
#include <gui/GuiSound.h>
#include <gui/GuiTextureData.h>

class HomebrewLaunchWindow : public GuiFrame, public sigslot::has_slots<> {
public:
    HomebrewLaunchWindow(const std::string & launchPath, GuiTextureData * iconImgData);
    virtual ~HomebrewLaunchWindow();

    sigslot::signal1<GuiElement *> backButtonClicked;
private:
    void OnBackButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger) {
        backButtonClicked(this);
    }

    void OnLoadButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);

    void OnFileLoadFinish(GuiElement *element, const std::string & filepath, int result);
    void OnOpenEffectFinish(GuiElement *element);
    void OnCloseEffectFinish(GuiElement *element);

    GuiSound *buttonClickSound;
    GuiTextureData * backgroundImgData;
    GuiImage backgroundImg;

    GuiTextureData *buttonImgData;
    GuiImage iconImage;

    GuiText titleText;
    GuiText versionText;
    GuiText versionValueText;
    GuiText authorText;
    GuiText authorValueText;
    GuiText descriptionText;

    GuiText loadBtnLabel;
    GuiImage loadImg;
    GuiButton loadBtn;

    GuiText backBtnLabel;
    GuiImage backImg;
    GuiButton backBtn;

    GuiTrigger touchTrigger;
    GuiTrigger wpadTouchTrigger;

    const std::string homebrewLaunchPath;
};

#endif //_HOMEBREW_LAUNCHER_WINDOW_H_
