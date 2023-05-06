#include "Behaviors.h"
#include <Arduino.h>
#include "Behaviors.h"

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

void Behaviors::SetIrrigationTime()
{
    //TODO
}

void Behaviors::SetNumberOfTimers(int number)
{
    Serial.println(number);
}

Behaviors::Behaviors(int solenoidValvePin)
{
    Behaviors::valveRelayPin = solenoidValvePin;
}
Behaviors::Behaviors() {}