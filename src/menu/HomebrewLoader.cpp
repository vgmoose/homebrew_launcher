#include <algorithm>
#include <string>
#include <string.h>
#include "HomebrewLoader.h"
#include "fs/CFile.hpp"
#include "utils/StringTools.h"

HomebrewLoader * HomebrewLoader::loadToMemoryAsync(const std::string & file) {
    HomebrewLoader * loader = new HomebrewLoader(file);
    // loader->resumeThread();
    return loader;
}

void HomebrewLoader::executeThread() {
    int result = loadToMemory();
    asyncLoadFinished(this, filepath, result);
}

int32_t HomebrewLoader::loadToMemory(const std::string & file) {
    HomebrewLoader * loader = new HomebrewLoader(file);
    int result = loader->loadToMemory();
    delete loader;
    return result;
}

// You must free the result if result is non-NULL.
char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = (char*)malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}

typedef struct __attribute((packed)) {
    uint32_t command;
    uint32_t target;
    uint32_t filesize;
    uint32_t fileoffset;
    char path[256];    
}LOAD_REQUEST;

int HomebrewLoader::loadToMemory() {
    if(filepath.empty()) {
        return INVALID_INPUT;
    }

    printf("Loading file %s\n", filepath.c_str());

    char * repl = (char*)"fs:/vol/external01/";
    char * with = (char*)"";
    char * input = (char*) filepath.c_str();

    char* extension = input + strlen(input) - 4;
    if (extension[0] == '.' &&
            extension[1] == 'r' &&
            extension[2] == 'p' &&
            extension[3] == 'x') {

        char * path = str_replace(input,repl, with);
        if(path != NULL) {
            LOAD_REQUEST request;
            memset(&request, 0, sizeof(request));
            
            printf("Loading file %s\n", path);
            request.command = 0xFC; // IPC_CUSTOM_LOAD_CUSTOM_RPX;
            request.target = 0;     // LOAD_FILE_TARGET_SD_CARD
            request.filesize = 0;   // unknown
            request.fileoffset = 0; //
            
            strncpy(request.path, path, 255);

            free(path);

            // TODO: launch using libwuhbtools / rpxloader
        }
    }

    return true;
}
