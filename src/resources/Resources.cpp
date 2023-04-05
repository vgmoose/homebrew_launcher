#include <malloc.h>
#include <string.h>
#include "Resources.h"
#include "fs/FSUtils.h"
// #include <gui/GuiImageAsync.h>
#include <gui/GuiSound.h>

Resources * Resources::instance = NULL;

void Resources::Clear() {
    // for(int i = 0; RecourceList[i].filename != NULL; ++i) {
    //     if(RecourceList[i].CustomFile) {
    //         free(RecourceList[i].CustomFile);
    //         RecourceList[i].CustomFile = NULL;
    //     }

    //     if(RecourceList[i].CustomFileSize != 0)
    //         RecourceList[i].CustomFileSize = 0;
    // }

    if(instance)
        delete instance;

    instance = NULL;
}

bool Resources::LoadFiles(const char * path) {
    if(!path)
        return false;

    bool result = false;
    Clear();

    // for(int i = 0; RecourceList[i].filename != NULL; ++i) {
    //     std::string fullpath(path);
    //     fullpath += "/";
    //     fullpath += RecourceList[i].filename;

    //     uint8_t * buffer = NULL;
    //     uint32_t filesize = 0;

    //     FSUtils::LoadFileToMem(fullpath.c_str(), &buffer, &filesize);

    //     RecourceList[i].CustomFile = buffer;
    //     RecourceList[i].CustomFileSize = (uint32_t) filesize;
    //     result |= (buffer != 0);
    // }

    return result;
}

uint8_t * Resources::GetFile(const char * filename) {
    // for(int i = 0; RecourceList[i].filename != NULL; ++i) {
    //     if(strcasecmp(filename, RecourceList[i].filename) == 0) {
    //         return (uint8_t*)(RecourceList[i].CustomFile ? RecourceList[i].CustomFile : RecourceList[i].DefaultFile);
    //     }
    // }

    return NULL;
}

uint32_t Resources::GetFileSize(const char * filename) {
    // for(int i = 0; RecourceList[i].filename != NULL; ++i) {
    //     if(strcasecmp(filename, RecourceList[i].filename) == 0) {
    //         return (RecourceList[i].CustomFile ? RecourceList[i].CustomFileSize : RecourceList[i].DefaultFileSize);
    //     }
    // }
    return 0;
}

GuiTextureData * Resources::GetImageData(const char * filename) {
    if(!instance)
        instance = new Resources;

    std::map<std::string, std::pair<unsigned int, GuiTextureData *> >::iterator itr = instance->imageDataMap.find(std::string(filename));
    if(itr != instance->imageDataMap.end()) {
        itr->second.first++;
        return itr->second.second;
    }

    // for(int i = 0; RecourceList[i].filename != NULL; ++i) {
    //     if(strcasecmp(filename, RecourceList[i].filename) == 0) {
    //         uint8_t * buff = (uint8_t *)(RecourceList[i].CustomFile ? RecourceList[i].CustomFile : RecourceList[i].DefaultFile);
    //         const uint32_t size = RecourceList[i].CustomFile ? RecourceList[i].CustomFileSize : RecourceList[i].DefaultFileSize;

    //         if(buff == NULL)
    //             return NULL;

    //         GuiTextureData * image = new GuiTextureData(buff, size);
    //         instance->imageDataMap[std::string(filename)].first = 1;
    //         instance->imageDataMap[std::string(filename)].second = image;

    //         return image;
    //     }
    // }

    return NULL;
}

void Resources::RemoveImageData(GuiTextureData * image) {
    std::map<std::string, std::pair<unsigned int, GuiTextureData *> >::iterator itr;

    for(itr = instance->imageDataMap.begin(); itr != instance->imageDataMap.end(); itr++) {
        if(itr->second.second == image) {
            itr->second.first--;

            if(itr->second.first == 0) {
                // AsyncDeleter::pushForDelete( itr->second.second );
                instance->imageDataMap.erase(itr);
            }
            break;
        }
    }
}

GuiSound * Resources::GetSound(const char * filename) {
    if(!instance)
        instance = new Resources;

    std::map<std::string, std::pair<unsigned int, GuiSound *> >::iterator itr = instance->soundDataMap.find(std::string(filename));
    if(itr != instance->soundDataMap.end()) {
        itr->second.first++;
        return itr->second.second;
    }

    // for(int i = 0; RecourceList[i].filename != NULL; ++i) {
    //     if(strcasecmp(filename, RecourceList[i].filename) == 0) {
    //         uint8_t * buff = (uint8_t *)(RecourceList[i].CustomFile ? RecourceList[i].CustomFile : RecourceList[i].DefaultFile);
    //         const uint32_t size = RecourceList[i].CustomFile ? RecourceList[i].CustomFileSize : RecourceList[i].DefaultFileSize;

    //         if(buff == NULL)
    //             return NULL;

    //         GuiSound * sound = new GuiSound(buff, size);
    //         instance->soundDataMap[std::string(filename)].first = 1;
    //         instance->soundDataMap[std::string(filename)].second = sound;

    //         return sound;
    //     }
    // }

    return NULL;
}

void Resources::RemoveSound(GuiSound * sound) {
    std::map<std::string, std::pair<unsigned int, GuiSound *> >::iterator itr;

    for(itr = instance->soundDataMap.begin(); itr != instance->soundDataMap.end(); itr++) {
        if(itr->second.second == sound) {
            itr->second.first--;

            if(itr->second.first == 0) {
                // AsyncDeleter::pushForDelete( itr->second.second );
                instance->soundDataMap.erase(itr);
            }
            break;
        }
    }
}
