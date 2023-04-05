#ifndef _HOMEBREW_WINDOW_H_
#define _HOMEBREW_WINDOW_H_

#include <gui/GuiFrame.h>
#include <gui/GuiButton.h>
#include <gui/GuiText.h>
#include <gui/GuiImage.h>
#include <gui/GuiTrigger.h>
#include <gui/GuiSound.h>
#include <gui/GuiTextureData.h>
#include <gui/GuiController.h>

class HomebrewWindow : public GuiFrame, public sigslot::has_slots<> {
public:
    HomebrewWindow(int w, int h);
    virtual ~HomebrewWindow();

    void draw(Renderer *pVideo);

private:
    void OnOpenEffectFinish(GuiElement *element);
    void OnCloseEffectFinish(GuiElement *element);
    void OnLaunchBoxCloseClick(GuiElement *element);
    void OnHomebrewButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
    void OnLeftArrowClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);
    void OnRightArrowClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger);

    GuiSound *buttonClickSound;
    GuiTextureData * homebrewButtonImgData;

    GuiTextureData* arrowRightImageData;
    GuiTextureData* arrowLeftImageData;
    GuiImage arrowRightImage;
    GuiImage arrowLeftImage;
    GuiButton arrowRightButton;
    GuiButton arrowLeftButton;
    GuiText hblVersionText;

    typedef struct {
        std::string execPath;
        GuiImage *image;
        GuiButton *button;
        GuiText *nameLabel;
        GuiText *descriptionLabel;
        GuiTextureData *iconImgData;
        GuiImage *iconImg;
    } homebrewButton;

    std::vector<homebrewButton> homebrewButtons;
    GuiTrigger touchTrigger;
    GuiTrigger wpadTouchTrigger;
    GuiTrigger buttonLTrigger;
    GuiTrigger buttonRTrigger;
    int listOffset;
    int currentLeftPosition;
    int targetLeftPosition;
};

#endif //_HOMEBREW_WINDOW_H_
