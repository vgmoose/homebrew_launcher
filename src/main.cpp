#include "Application.h"
#include "utils/utils.h"
#include "common/common.h"

int main(int argc, char **argv) {

    printf("Welcome to the Homebrew Launcher %s\n", HBL_VERSION);
    
    printf("Start main application\n");
    int returnCode = Application::instance()->exec();
    printf("Main application stopped\n");

    Application::destroyInstance();

    printf("HBL exit\n");

    return returnCode;
}

