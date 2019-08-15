#include <nsysnet/socket.h>
#include "Application.h"
#include "system/memory.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "common/common.h"

/* Entry point */
extern "C" int Menu_Main(void) {
    //!*******************************************************************
    //!                   Initialize function pointers                   *
    //!*******************************************************************
    socket_lib_init();
    //! do OS (for acquire) and sockets first so we got logging
    log_init();


    log_printf("Welcome to the Homebrew Launcher %s\n", HBL_VERSION);

    //!*******************************************************************
    //!                    Enter main application                        *
    //!*******************************************************************
    log_printf("Start main application\n");
    int returnCode = Application::instance()->exec();
    log_printf("Main application stopped\n");

    Application::destroyInstance();

    log_printf("HBL exit\n");

    return returnCode;
}

