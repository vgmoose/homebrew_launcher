#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <vector>
#include <queue>
#include "HomebrewWindow.h"

class MainWindow : public sigslot::has_slots<> {
public:
    MainWindow(int w, int h);
    virtual ~MainWindow();

    void appendTv(GuiElement *e) {
        if(!e)
            return;

        removeTv(e);
        tvElements.push_back(e);
    }
    void appendDrc(GuiElement *e) {
        if(!e)
            return;

        removeDrc(e);
        drcElements.push_back(e);
    }

    void append(GuiElement *e) {
        appendTv(e);
        appendDrc(e);
    }

    void insertTv(uint32_t pos, GuiElement *e) {
        if(!e)
            return;

        removeTv(e);
        tvElements.insert(tvElements.begin() + pos, e);
    }
    void insertDrc(uint32_t pos, GuiElement *e) {
        if(!e)
            return;

        removeDrc(e);
        drcElements.insert(drcElements.begin() + pos, e);
    }

    void insert(uint32_t pos, GuiElement *e) {
        insertTv(pos, e);
        insertDrc(pos, e);
    }

    void removeTv(GuiElement *e) {
        for(uint32_t i = 0; i < tvElements.size(); ++i) {
            if(e == tvElements[i]) {
                tvElements.erase(tvElements.begin() + i);
                break;
            }
        }
    }
    void removeDrc(GuiElement *e) {
        for(uint32_t i = 0; i < drcElements.size(); ++i) {
            if(e == drcElements[i]) {
                drcElements.erase(drcElements.begin() + i);
                break;
            }
        }
    }

    void remove(GuiElement *e) {
        removeTv(e);
        removeDrc(e);
    }
    void removeAll() {
        tvElements.clear();
        drcElements.clear();
    }

    void drawDrc(Renderer *video);
    void drawTv(Renderer *video);
    void update(GuiController *controller);
    void updateEffects();
private:
    int width, height;
    std::vector<GuiElement *> drcElements;
    std::vector<GuiElement *> tvElements;

    GuiImage bgImageColor;
    // GuiParticleImage bgParticleImg;
    HomebrewWindow homebrewWindow;

    GuiTextureData *pointerImgData[4];
    GuiImage *pointerImg[4];
    bool pointerValid[4];
};

#endif //_MAIN_WINDOW_H_
