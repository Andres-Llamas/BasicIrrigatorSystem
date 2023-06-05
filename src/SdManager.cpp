#include "SdManager.h"

void SdManager::Initialize()
{
    if (!SD.begin(5))
    {
        Serial.println("Card Mount Failed");
        return;
    }
}

void SdManager::CreateDir(char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (SD.mkdir(path))
    {
        Serial.println("Dir created");
    }
    else
    {
        Serial.println("mkdir failed");
    }
}

void SdManager::RemoveDir(char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (SD.rmdir(path))
    {
        Serial.println("Dir removed");
    }
    else
    {
        Serial.println("rmdir failed");
    }
}

void SdManager::WriteFile(char *path, char *message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = SD.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
}

String SdManager::ReadFile(char *path)
{
    Serial.printf("Reading file: %s\n", path);

    File file = SD.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
    }

    Serial.print("Read from file: ");
    while (file.available())
    {
        Serial.write(file.read());
    }
    return file.readString();
    file.close();
}

void SdManager::AppendFile(const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);

    File file = SD.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}

void SdManager::DeleteFile(char *path)
{
    Serial.printf("Deleting file: %s\n", path);
    if (SD.remove(path))
    {
        Serial.println("File deleted");
    }
    else
    {
        Serial.println("Delete failed");
    }
}

SdManager::SdManager()
{
    
}