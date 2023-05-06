#ifndef WifiServerManager_h
#define WifiServerManager_h
#include "Behaviors.h"

class WifiServerManager
{
private:
    /*When the user enters "http://sisriego.local/timers", the user must write the parameters as.-
    http://sisriego.local/timers?hour=val1&minute=val2&second=val3
    then this method will count how many parameters there are, then will iterate for each one, if the name matches with the 
    name of the variable from this method, then gets the value written from the user and stores it in a string, then after the 
    iteration is done, this method will convert each string to int and store then uin the "clockTime" structure in order to pass that 
    structure to the method "AddIrrigatorTimer" from "Behaviors.h"
    */
    void SetNumberOfTimersHandler();
    void GetClockTimeFromListHandler();

public:
    void Initialize();
    void StablishMDNSDirectionsAndBeginServer();
    void UpdateServerCLient();
    WifiServerManager(char *wifiName, char *wifiPassword, Behaviors behaviorsObject);
};
#endif