#ifndef SdManager_h
#define SdManager_h
#include "FS.h"
#include "SD.h"
#include "SPI.h"
class SdManager
{
private:
    /* data */
public:
    void Initialize();
    void CreateDir(char *path);
    void RemoveDir(char *path);
    void WriteFile(char *path, char *message);
    String ReadFile(char *path);
    void AppendFile(const char *path, const char *message);
    void DeleteFile(char *path);

    SdManager(/* args */);
};
#endif