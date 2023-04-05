/****************************************************************************
 * Copyright (C) 2015 Dimok
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ****************************************************************************/
#include "HomebrewWindow.h"
#include "common/common.h"
#include "Application.h"
#include "fs/DirList.h"
#include "fs/FSUtils.h"
#include "utils/HomebrewXML.h"
#include "resources/Resources.h"
#include "utils/utils.h"
#include "HomebrewLaunchWindow.h"
#include "HomebrewLoader.h"

#define DEFAULT_WIILOAD_PORT        4299

#define MAX_BUTTONS_ON_PAGE     4

#define sdlWhite               (SDL_Color) {255, 255, 255, 255}

HomebrewWindow::HomebrewWindow(int w, int h)
    : GuiFrame(w, h)
    , buttonClickSound(Resources::GetSound("button_click.mp3"))
    , homebrewButtonImgData(Resources::GetImageData("homebrewButton.png"))
    , arrowRightImageData(Resources::GetImageData("rightArrow.png"))
    , arrowLeftImageData(Resources::GetImageData("leftArrow.png"))
    , arrowRightImage(arrowRightImageData)
    , arrowLeftImage(arrowLeftImageData)
    , arrowRightButton(arrowRightImage.getWidth(), arrowRightImage.getHeight())
    , arrowLeftButton(arrowLeftImage.getWidth(), arrowLeftImage.getHeight())
    , hblVersionText("Homebrew Launcher v3 (" HBL_VERSION ") by Dimok and friends", 32, sdlWhite)
    , touchTrigger(GuiTrigger::CHANNEL_1, GuiTrigger::VPAD_TOUCH)
    , wpadTouchTrigger(GuiTrigger::CHANNEL_2 | GuiTrigger::CHANNEL_3 | GuiTrigger::CHANNEL_4 | GuiTrigger::CHANNEL_5, GuiTrigger::BUTTON_A)
    , buttonLTrigger(GuiTrigger::CHANNEL_ALL, GuiTrigger::BUTTON_L | GuiTrigger::BUTTON_LEFT, true)
    , buttonRTrigger(GuiTrigger::CHANNEL_ALL, GuiTrigger::BUTTON_R | GuiTrigger::BUTTON_RIGHT, true) {
        
    targetLeftPosition = 0;
    currentLeftPosition = 0;
    listOffset = 0;

    DirList dirList("fs:/vol/external01/wiiu/apps", ".rpx", DirList::Files | DirList::CheckSubfolders, 1);

    dirList.SortList();

    for(int i = 0; i < dirList.GetFilecount(); i++) {
        //! skip our own application in the listing
        if(strcasecmp(dirList.GetFilename(i), "homebrew_launcher.elf") == 0)
            continue;
        //! skip our own application in the listing
        if(strcasecmp(dirList.GetFilename(i), "homebrew_launcher.rpx") == 0)
            continue;


        //! skip hidden linux and mac files
        if(dirList.GetFilename(i)[0] == '.' || dirList.GetFilename(i)[0] == '_')
            continue;

        int idx = homebrewButtons.size();
        homebrewButtons.resize(homebrewButtons.size() + 1);

        homebrewButtons[idx].execPath = dirList.GetFilepath(i);
        homebrewButtons[idx].image = new GuiImage(homebrewButtonImgData);
        homebrewButtons[idx].image->setScale(0.9f);
        homebrewButtons[idx].iconImgData = NULL;

        std::string homebrewPath = homebrewButtons[idx].execPath;
        size_t slashPos = homebrewPath.rfind('/');
        if(slashPos != std::string::npos)
            homebrewPath.erase(slashPos);

        uint8_t * iconData = NULL;
        uint32_t iconDataSize = 0;

        FSUtils::LoadFileToMem((homebrewPath + "/icon.png").c_str(), &iconData, &iconDataSize);

        if(iconData != NULL) {
            homebrewButtons[idx].iconImgData = new GuiTextureData(iconData, iconDataSize);
            free(iconData);
            iconData = NULL;
        }

        const float cfImageScale = 1.0f;

        homebrewButtons[idx].iconImg = new GuiImage(homebrewButtons[idx].iconImgData);
        homebrewButtons[idx].iconImg->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
        homebrewButtons[idx].iconImg->setPosition(60, 0);
        homebrewButtons[idx].iconImg->setScale(cfImageScale);

        HomebrewXML metaXml;

        bool xmlReadSuccess = metaXml.LoadHomebrewXMLData((homebrewPath + "/meta.xml").c_str());

        const char *cpName = xmlReadSuccess ? metaXml.GetName() : homebrewButtons[idx].execPath.c_str();
        const char *cpDescription = xmlReadSuccess ? metaXml.GetShortDescription() : "";

        if(strncmp(cpName, "fs:/vol/external01/wiiu/apps/", strlen("fs:/vol/external01/wiiu/apps/")) == 0)
            cpName += strlen("fs:/vol/external01/wiiu/apps/");

        homebrewButtons[idx].nameLabel = new GuiText(cpName, 32, sdlWhite);
        homebrewButtons[idx].nameLabel->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
        homebrewButtons[idx].nameLabel->setMaxWidth(350);
        homebrewButtons[idx].nameLabel->setPosition(256 + 80, 20);

        homebrewButtons[idx].descriptionLabel = new GuiText(cpDescription, 32, sdlWhite);
        homebrewButtons[idx].descriptionLabel->setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
        homebrewButtons[idx].descriptionLabel->setMaxWidth(350);
        homebrewButtons[idx].descriptionLabel->setPosition(256 + 80, -20);

        homebrewButtons[idx].button = new GuiButton(homebrewButtonImgData->getWidth(), homebrewButtonImgData->getHeight());

        homebrewButtons[idx].button->setImage(homebrewButtons[idx].image);
        homebrewButtons[idx].button->setLabel(homebrewButtons[idx].nameLabel, 0);
        homebrewButtons[idx].button->setLabel(homebrewButtons[idx].descriptionLabel, 1);
        homebrewButtons[idx].button->setIcon(homebrewButtons[idx].iconImg);
        float fXOffset = (idx / MAX_BUTTONS_ON_PAGE) * width;
        float fYOffset = (homebrewButtons[idx].image->getHeight() + 20.0f) * 1.5f - (homebrewButtons[idx].image->getHeight() + 20) * (idx % MAX_BUTTONS_ON_PAGE);
        homebrewButtons[idx].button->setPosition(currentLeftPosition + fXOffset, fYOffset);
        homebrewButtons[idx].button->setTrigger(&touchTrigger);
        homebrewButtons[idx].button->setTrigger(&wpadTouchTrigger);
        homebrewButtons[idx].button->setEffectGrow();
        homebrewButtons[idx].button->setSoundClick(buttonClickSound);
        homebrewButtons[idx].button->clicked.connect(this, &HomebrewWindow::OnHomebrewButtonClick);

        append(homebrewButtons[idx].button);
    }


    if((MAX_BUTTONS_ON_PAGE) < homebrewButtons.size()) {
        arrowLeftButton.setImage(&arrowLeftImage);
        arrowLeftButton.setEffectGrow();
        arrowLeftButton.setPosition(40, 0);
        arrowLeftButton.setAlignment(ALIGN_LEFT | ALIGN_MIDDLE);
        arrowLeftButton.setTrigger(&touchTrigger);
        arrowLeftButton.setTrigger(&wpadTouchTrigger);
        arrowLeftButton.setTrigger(&buttonLTrigger);
        arrowLeftButton.setSoundClick(buttonClickSound);
        arrowLeftButton.clicked.connect(this, &HomebrewWindow::OnLeftArrowClick);

        arrowRightButton.setImage(&arrowRightImage);
        arrowRightButton.setEffectGrow();
        arrowRightButton.setPosition(-40, 0);
        arrowRightButton.setAlignment(ALIGN_RIGHT | ALIGN_MIDDLE);
        arrowRightButton.setTrigger(&touchTrigger);
        arrowRightButton.setTrigger(&wpadTouchTrigger);
        arrowRightButton.setTrigger(&buttonRTrigger);
        arrowRightButton.setSoundClick(buttonClickSound);
        arrowRightButton.clicked.connect(this, &HomebrewWindow::OnRightArrowClick);
        append(&arrowRightButton);
    }

    hblVersionText.setAlignment(ALIGN_BOTTOM | ALIGN_RIGHT);
    hblVersionText.setPosition(-30, 30);
    append(&hblVersionText);
}

HomebrewWindow::~HomebrewWindow() {
    for(uint32_t i = 0; i < homebrewButtons.size(); ++i) {
        delete homebrewButtons[i].image;
        delete homebrewButtons[i].nameLabel;
        delete homebrewButtons[i].descriptionLabel;
        delete homebrewButtons[i].button;
        delete homebrewButtons[i].iconImgData;
        delete homebrewButtons[i].iconImg;
    }

    Resources::RemoveSound(buttonClickSound);
    Resources::RemoveImageData(homebrewButtonImgData);
    Resources::RemoveImageData(arrowRightImageData);
    Resources::RemoveImageData(arrowLeftImageData);
}

void HomebrewWindow::OnOpenEffectFinish(GuiElement *element) {
    //! once the menu is open reset its state and allow it to be "clicked/hold"
    element->effectFinished.disconnect(this);
    element->clearState(GuiElement::STATE_DISABLED, -1);
}

void HomebrewWindow::OnCloseEffectFinish(GuiElement *element) {
    //! remove element from draw list and push to delete queue
    remove(element);
    // AsyncDeleter::pushForDelete(element);

    for(uint32_t i = 0; i < homebrewButtons.size(); i++) {
        homebrewButtons[i].button->clearState(GuiElement::STATE_DISABLED, -1);
    }
}

void HomebrewWindow::OnLaunchBoxCloseClick(GuiElement *element) {
    element->setState(GuiElement::STATE_DISABLED, -1);
    element->setEffect(EFFECT_FADE, -10, 0);
    element->effectFinished.connect(this, &HomebrewWindow::OnCloseEffectFinish);
}

void HomebrewWindow::OnHomebrewButtonClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger) {
    bool disableButtons = false;

    for(uint32_t i = 0; i < homebrewButtons.size(); i++) {
        if(button == homebrewButtons[i].button) {
            HomebrewLaunchWindow * launchBox = new HomebrewLaunchWindow(homebrewButtons[i].execPath, homebrewButtons[i].iconImgData);
            launchBox->setEffect(EFFECT_FADE, 10, 255);
            launchBox->setState(GuiElement::STATE_DISABLED, -1);
            launchBox->setPosition(0.0f, 30.0f);
            launchBox->effectFinished.connect(this, &HomebrewWindow::OnOpenEffectFinish);
            launchBox->backButtonClicked.connect(this, &HomebrewWindow::OnLaunchBoxCloseClick);
            append(launchBox);
            disableButtons = true;
            break;
        }
    }


    if(disableButtons) {
        for(uint32_t i = 0; i < homebrewButtons.size(); i++) {
            homebrewButtons[i].button->setState(GuiElement::STATE_DISABLED, -1);
        }
    }
}

void HomebrewWindow::OnLeftArrowClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger) {
    if(listOffset > 0) {
        listOffset--;
        targetLeftPosition = -listOffset * getWidth();

        if(listOffset == 0)
            remove(&arrowLeftButton);
        append(&arrowRightButton);
    }
}

void HomebrewWindow::OnRightArrowClick(GuiButton *button, const GuiController *controller, GuiTrigger *trigger) {
    if((listOffset * MAX_BUTTONS_ON_PAGE) < (int)homebrewButtons.size()) {
        listOffset++;
        targetLeftPosition = -listOffset * getWidth();

        if(((listOffset + 1) * MAX_BUTTONS_ON_PAGE) >= (int)homebrewButtons.size())
            remove(&arrowRightButton);

        append(&arrowLeftButton);
    }
}

void HomebrewWindow::draw(Renderer *pVideo) {
    bool bUpdatePositions = false;

    if(currentLeftPosition < targetLeftPosition) {
        currentLeftPosition += 35;

        if(currentLeftPosition > targetLeftPosition)
            currentLeftPosition = targetLeftPosition;

        bUpdatePositions = true;
    } else if(currentLeftPosition > targetLeftPosition) {
        currentLeftPosition -= 35;

        if(currentLeftPosition < targetLeftPosition)
            currentLeftPosition = targetLeftPosition;

        bUpdatePositions = true;
    }

    if(bUpdatePositions) {
        bUpdatePositions = false;

        for(uint32_t i = 0; i < homebrewButtons.size(); i++) {
            float fXOffset = (i / MAX_BUTTONS_ON_PAGE) * getWidth();
            float fYOffset = (homebrewButtons[i].image->getHeight() + 20.0f) * 1.5f - (homebrewButtons[i].image->getHeight() + 20) * (i % MAX_BUTTONS_ON_PAGE);
            homebrewButtons[i].button->setPosition(currentLeftPosition + fXOffset, fYOffset);
        }
    }

    GuiFrame::draw(pVideo);
}
