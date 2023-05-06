#include "Behaviors.h"
#include <Arduino.h>
#include "Behaviors.h"
#include <list>
#include "TimeManager.h"

clockTime timersToActivate [5];

void Behaviors::Initialization()
{
    pinMode(Behaviors::valveRelayPin, OUTPUT);
    digitalWrite(Behaviors::valveRelayPin, LOW);
}

void Behaviors::SetIrrigationActive(bool state)
{
    digitalWrite(Behaviors::valveRelayPin, state);
    if (state)
        Serial.println("Irrigation was started");
    else
        Serial.println("Irrigation was stopped");
}

void Behaviors::AddIrrigatorTimer(clockTime timeToActivate, int indexToSet)
{
    // this method adds clockTime structures which contain day, hours, minutes and seconds to the list in order to make a
    // register of the hours the user wants the irrigator system to turn on
    timersToActivate[indexToSet] = timeToActivate;
    Serial.println("The following data has been stored in the clock system index " + indexToSet);
    TimeManager::ShowDateAndTime(timeToActivate);
}

clockTime Behaviors::GetClockTimeFromList(int index)
{
    clockTime val = timersToActivate[index];
    TimeManager::ShowDateAndTime(val);
    return val;
}


Behaviors::Behaviors(int solenoidValvePin)
{
    Behaviors::valveRelayPin = solenoidValvePin;
}
Behaviors::Behaviors() {}