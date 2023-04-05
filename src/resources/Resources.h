#ifndef RECOURCES_H_
#define RECOURCES_H_

#include <map>
#include <string>
#include <stdint.h>
#include <gui/GuiTextureData.h>
#include <gui/GuiSound.h>

class Resources {
public:
    static void Clear();
    static bool LoadFiles(const char * path);
    static uint8_t * GetFile(const char * filename);
    static uint32_t GetFileSize(const char * filename);

    static GuiTextureData * GetImageData(const char * filename);
    static void RemoveImageData(GuiTextureData * image);

    static GuiSound * GetSound(const char * filename);
    static void RemoveSound(GuiSound * sound);
private:
    static Resources *instance;

    Resources() {}
    ~Resources() {}

    std::map<std::string, std::pair<unsigned int, GuiTextureData *> > imageDataMap;
    std::map<std::string, std::pair<unsigned int, GuiSound *> > soundDataMap;
};

#endif
