#include "Application.h"
#include "common/common.h"
#include "resources/Resources.h"

#include <gui/GuiController.h>
#include <gui/input/SDLControllerWiiUProContoller.h>
#include <gui/input/SDLControllerWiiUGamepad.h>
#include <gui/system/SDLSystem.h>

// #include <gui/sounds/SoundHandler.hpp>

Application *Application::applicationInstance = NULL;
bool Application::exitApplication = false;
bool Application::quitRequest = false;

Application::Application()
    // : CThread(CThread::eAttributeAffCore1 | CThread::eAttributePinnedAff, 0, 0x20000)
    : bgMusic(NULL)
    , video(NULL)
    , mainWindow(NULL)
    , fontSystem(NULL)
    , exitCode(EXIT_RELAUNCH_ON_LOAD) {
        // TODO: handle wiiu controls better?
    controller[0] = new SDLControllerWiiUGamepad(GuiTrigger::CHANNEL_1);
    controller[1] = new SDLControllerWiiUProContoller(GuiTrigger::CHANNEL_2);
    controller[2] = new SDLControllerWiiUProContoller(GuiTrigger::CHANNEL_3);
    controller[3] = new SDLControllerWiiUProContoller(GuiTrigger::CHANNEL_4);
    controller[4] = new SDLControllerWiiUProContoller(GuiTrigger::CHANNEL_5);

    //! load resources
    Resources::LoadFiles("fs:/vol/external01/wiiu/apps/homebrew_launcher/resources");

    //! create bgMusic
    bgMusic = new GuiSound(Resources::GetFile("bgMusic.ogg"), Resources::GetFileSize("bgMusic.ogg"));
    bgMusic->SetLoop(true);
    bgMusic->Play();
    bgMusic->SetVolume(50);

    exitApplication = false;

#ifndef PC
    ProcUIInit(OSSavesDone_ReadyToRelease);
#endif
}

Application::~Application() {
    printf("Destroy music\n");

    delete bgMusic;

    printf("Destroy controller\n");

    for(int i = 0; i < 5; i++)
        delete controller[i];

    // printf("Destroy async deleter\n");
    // AsyncDeleter::destroyInstance();

    printf("Clear resources\n");
    Resources::Clear();

    // printf("Stop sound handler\n");
    // SoundHandler::DestroyInstance();

#ifndef PC
    ProcUIShutdown();
#endif
}

int Application::exec() {
    //! start main GX2 thread
    // resumeThread();
    //! now wait for thread to finish
    // shutdownThread();

    return exitCode;
}

void Application::quit(int code) {
    exitCode = code;
    exitApplication = true;
    quitRequest = true;
}

void Application::fadeOut() {
#ifndef PC
    GuiImage fadeOut(video->getTvWidth(), video->getTvHeight(), (SDL_Color) {
        0, 0, 0, 255
    });

    for(int i = 0; i < 255; i += 10) {
        if(i > 255)
            i = 255;

        fadeOut.setAlpha(i / 255.0f);

        //! start rendering DRC
        // video->prepareDrcRendering();
        mainWindow->drawDrc(video);

        GX2SetDepthOnlyControl(GX2_DISABLE, GX2_DISABLE, GX2_COMPARE_FUNC_ALWAYS);
        fadeOut.draw(video);
        GX2SetDepthOnlyControl(GX2_ENABLE, GX2_ENABLE, GX2_COMPARE_FUNC_LEQUAL);

        // video->drcDrawDone();

        //! start rendering TV
        // video->prepareTvRendering();

        mainWindow->drawTv(video);

        GX2SetDepthOnlyControl(GX2_DISABLE, GX2_DISABLE, GX2_COMPARE_FUNC_ALWAYS);
        fadeOut.draw(video);
        GX2SetDepthOnlyControl(GX2_ENABLE, GX2_ENABLE, GX2_COMPARE_FUNC_LEQUAL);

        // video->tvDrawDone();

        //! as last point update the effects as it can drop elements
        mainWindow->updateEffects();

        // video->waitForVSync();
    }

    //! one last cleared black screen
    // video->prepareDrcRendering();
    // video->drcDrawDone();
    // video->prepareTvRendering();
    // video->tvDrawDone();
    // video->waitForVSync();
    // video->tvEnable(false);
    // video->drcEnable(false);
#endif
}

bool Application::procUI(void) {
    bool executeProcess = false;

#ifndef PC
    switch(ProcUIProcessMessages(true)) {
    case PROCUI_STATUS_EXITING: {
        printf("PROCUI_STATUS_EXITING\n");
        exitCode = EXIT_SUCCESS;
        exitApplication = true;
        break;
    }
    case PROCUI_STATUS_RELEASE_FOREGROUND: {
        printf("PROCUI_STATUS_RELEASE_FOREGROUND\n");
        if(video != NULL) {
            // we can turn of the screen but we don't need to and it will display the last image
            video->tvEnable(true);
            video->drcEnable(true);

            printf("delete fontSystem\n");
            delete fontSystem;
            fontSystem = NULL;

            printf("delete video\n");
            delete video;
            video = NULL;
        }
        ProcUIDrawDoneRelease();
        break;
    }
    case PROCUI_STATUS_IN_FOREGROUND: {
        if(!quitRequest) {
            printf("PROCUI_STATUS_IN_FOREGROUND\n");
            executeProcess = true;
        }
        break;
    }
    case PROCUI_STATUS_IN_BACKGROUND:
    default:
        break;
    }
#endif

    return executeProcess;
}

// TODO: actually measure TV on wiiu
int getTvWidth() {
    return 1280;
}

int getTvHeight() {
    return 720;
}

void Application::executeThread(void) {
    printf("Entering main loop\n");

    // printf("Initialize video\n");
    SDLSystem sdlSystem;
    Renderer* mainRenderer = sdlSystem.getRenderer();
    video = new Renderer(mainRenderer->getRenderer(), SDL_PIXELFORMAT_RGBA8888);
    printf("Video size %i x %i\n", getTvWidth(), getTvHeight());

    //! setup default Font
    printf("Initialize main font system\n");
    GuiFont *fontSystem = new GuiFont(Resources::GetFile("font.ttf"), Resources::GetFileSize("font.ttf"), mainRenderer);
    GuiText::setPresetFont(fontSystem);

    if(mainWindow == NULL) {
        printf("Initialize main window\n");
        mainWindow = new MainWindow(getTvWidth(), getTvHeight());
    }
    //! main GX2 loop (60 Hz cycle with max priority on core 1)
    while(!exitApplication) {
        procUI();

        //! Read out inputs
        SDL_Event event;
        SDL_PollEvent(&event);
        for(int i = 0; i < 5; i++) {
            // TODO: oob check
            if(controller[i]->update(&event, getTvWidth(), getTvHeight()) == false)
                continue;

            //! update controller states
            mainWindow->update(controller[i]);
        }

        //! start rendering DRC
        // video->prepareDrcRendering();
        mainWindow->drawDrc(video);
        // video->drcDrawDone();

        //! start rendering TV
        // video->prepareTvRendering();
        mainWindow->drawTv(video);
        // video->tvDrawDone();

        //! enable screen after first frame render
        // if(video->getFrameCount() == 0) {
        //     video->tvEnable(true);
        //     video->drcEnable(true);
        // }

        //! as last point update the effects as it can drop elements
        mainWindow->updateEffects();

        // video->waitForVSync();

        //! transfer elements to real delete list here after all processes are finished
        //! the elements are transfered to another list to delete the elements in a separate thread
        //! and avoid blocking the GUI thread
        // AsyncDeleter::triggerDeleteProcess();
    }

    //! in case we exit to a homebrew let's smoothly fade out
    if(video) {
        fadeOut();
    }

    printf("delete mainWindow\n");
    delete mainWindow;
    mainWindow = NULL;

    printf("delete fontSystem\n");
    delete fontSystem;
    fontSystem = NULL;

    printf("delete video\n");
    delete video;
    video = NULL;
}
