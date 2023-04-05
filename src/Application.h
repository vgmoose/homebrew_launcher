#ifndef _APPLICATION_H
#define _APPLICATION_H

#include <gui/GuiButton.h>
#include <gui/input/SDLController.h>
#include "menu/MainWindow.h"

class Application {
public:
    static Application * instance() {
        if(!applicationInstance)
            applicationInstance = new Application();
        return applicationInstance;
    }
    static void destroyInstance() {
        if(applicationInstance) {
            delete applicationInstance;
            applicationInstance = NULL;
        }
    }

    Renderer *getVideo(void) const {
        return video;
    }
    MainWindow *getMainWindow(void) const {
        return mainWindow;
    }

    GuiSound *getBgMusic(void) const {
        return bgMusic;
    }

    void initElements();

    int exec(void);
    void fadeOut(void);

    void quit(int code);

private:
    Application();
    virtual ~Application();

    bool procUI(void);

    static Application *applicationInstance;
    static bool exitApplication;
    static bool quitRequest;

    void executeThread(void);

    GuiSound *bgMusic;
    Renderer *video;
    MainWindow *mainWindow;
    GuiFont *fontSystem;
    SDLController *controller[5];
    int exitCode;
};

#endif //_APPLICATION_H
