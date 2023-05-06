#ifndef WifiServerManager_h
#define WifiServerManager_h
#include "Behaviors.h"

class WifiServerManager
{
private:
    void SetNumberOfTimersHandler();

public:
    void Initialize();
    void StablishMDNSDirectionsAndBeginServer();
    void UpdateServerCLient();
    WifiServerManager(char *wifiName, char *wifiPassword, Behaviors behaviorsObject);
};
#endif