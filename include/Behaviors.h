#ifndef Behaviors_h
#define Behaviors_h

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
    void SetIrrigationTime();
    /// @brief This is to stablish how many times the irrigation system will be activated.
    /// basically, how many "alarms"
    /// @param numberOfTimers
    void SetNumberOfTimers(int number);
    Behaviors(int solenoidValvePin);
    Behaviors();
};
#endif