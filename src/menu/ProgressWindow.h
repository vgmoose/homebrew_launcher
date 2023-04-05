#ifndef _PROGRESS_WINDOW_H_
#define _PROGRESS_WINDOW_H_
#include <gui/GuiFrame.h>
#include <gui/GuiText.h>
#include <gui/GuiImage.h>
#include <gui/GuiButton.h>
#include <gui/GuiTrigger.h>
#include <gui/GuiSound.h>
#include <gui/sigslot.h>

class ProgressWindow : public GuiFrame, public sigslot::has_slots<> {
public:
    ProgressWindow(const std::string & titleText);
    virtual ~ProgressWindow();

    void setProgress(float percent);
    void setTitle(const std::string & title);
private:
    void draw(Renderer * v);

    GuiText titleText;
    GuiTextureData *bgImageData;
    GuiImage bgImage;
    GuiImage progressImageBlack;
    GuiImage progressImageColored;

    GuiTrigger touchTrigger;
    GuiTrigger wpadTouchTrigger;

    bool titleChanged;
    std::string currentTitle;
    // CMutex titleMutex;
};

#endif //_PROGRESS_WINDOW_H_
