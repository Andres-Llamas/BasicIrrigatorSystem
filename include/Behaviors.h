#ifndef Behaviors_h
#define Behaviors_h
#include "CustomStructs.h"

class Behaviors
{
private:
    /* data */
public:
    int valveRelayPin;
    void Initialization();

    /// @brief to initiate or stop irrigation manually
    /// @param state
    void SetIrrigationActive(bool state);

    /// @brief this method adds clockTime structures which contain day, hours, minutes and seconds to the list in order to make a 
    /// register of the hours the user wants the irrigator system to turn on
    /// @param timeToActivate 
    void AddIrrigatorTimer(clockTime timeToActivate, int indexToSet);

    clockTime GetClockTimeFromList(int idex);
    Behaviors(int solenoidValvePin);
    
    Behaviors();
};
#endif