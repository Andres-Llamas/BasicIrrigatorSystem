#include "Behaviors.h"
#include <Arduino.h>
#include "Behaviors.h"
#include <list>
#include "TimeManager.h"

clockTime timersToActivate[5];
clockTime timersToDeactivate[5];

void Behaviors::Initialization()
{
    pinMode(Behaviors::valveRelayPin, OUTPUT);
    digitalWrite(Behaviors::valveRelayPin, LOW);
}

void Behaviors::SetIrrigationActive(bool state)
{
    digitalWrite(Behaviors::valveRelayPin, state);
    Behaviors::irrigationValveIsActive = state;
    if (state)
        Serial.println("Irrigation was started");
    else
        Serial.println("Irrigation was stopped");
}

void Behaviors::AddIrrigatorTimer(clockTime timeToActivate, clockTime timeToStop, int indexToSet)
{
    // this method adds clockTime structures which contain day, hours, minutes and seconds to the list in order to make a
    // register of the hours the user wants the irrigator system to turn on
    timersToActivate[indexToSet] = timeToActivate;
    timersToDeactivate[indexToSet] = timeToStop;
    Serial.print("The following data has been stored in the clock system index ");
    Serial.println(indexToSet);
    Serial.print("Start time.- ");
    TimeManager::ShowDateAndTime(timeToActivate);
    Serial.print("Stop time.-  ");
    TimeManager::ShowDateAndTime(timeToStop);
    TimeManager::ShowDateAndTime();
}

void Behaviors::SetIrrigatorWithTimer(bool state)
{
    activateIrrigatorWithTimer = state;
    if (state)
        Serial.println("Irrigator activation mode has changed to : timer");
    else
        Serial.println("Irrigator activation mode has changed to : manual");
}

void Behaviors::CheckTimeForValveActivation()
{
    if (Behaviors::activateIrrigatorWithTimer)
    {

        for (int i = 0; i < 5; i++)
        {
            clockTime startTime = timersToActivate[i];
            clockTime stopTime = timersToDeactivate[i];
            if (startTime.hours != 0) // if the hours is 0, that means that index haven't been set by the user
            {
                if (Behaviors::irrigationValveIsActive == false) // if the valve is closed
                {
                    // Check at what time to open it according to the established by the user
                    if (startTime.hours == TimeManager::currentTime.hours && startTime.minutes == TimeManager::currentTime.minutes)
                    {
                        Serial.print("Activation from timer with index: ");
                        Serial.println(i);
                        TimeManager::ShowDateAndTime();
                        Behaviors::irrigationValveIsActive = true;
                        digitalWrite(Behaviors::valveRelayPin, HIGH);
                    }
                }
                else // if the valve is open
                {                    
                    // Check at what time to close it according to the established by the user
                    if (stopTime.hours == TimeManager::currentTime.hours && stopTime.minutes == TimeManager::currentTime.minutes)
                    {
                        Serial.print("Deactivation from timer with index: ");
                        Serial.println(i);
                        TimeManager::ShowDateAndTime();
                        Behaviors::irrigationValveIsActive = false;
                        digitalWrite(Behaviors::valveRelayPin, LOW);
                    }
                }
            }
        }
    }
}

void Behaviors::CheckIrrigationValveState()
{
    //TODO check a bug that even though the valve is open, the text that appears is the one that says is closed
    if(Behaviors::irrigationValveIsActive)
    {
        Serial.println("The solenoid valve is currently open and working!");
    }
    else
    {
        Serial.println("The solenoid valve is currently closed and sleeping!");
    }
}

clockTime Behaviors::GetClockTimeFromList(int index)
{
    clockTime val = timersToActivate[index];
    Serial.print("Getting timer from index ");
    Serial.println(index);
    TimeManager::ShowDateAndTime(val);
    TimeManager::ShowDateAndTime();
    return val;
}

Behaviors::Behaviors(int solenoidValvePin)
{
    Behaviors::valveRelayPin = solenoidValvePin;
}
Behaviors::Behaviors() {}