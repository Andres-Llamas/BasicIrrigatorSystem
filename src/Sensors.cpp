#include <Arduino.h>
#include "Sensors.h"
#include "TimeManager.h"

int Sensors::lightSensorPin = 35;
int Sensors::rainSensorAnalogPin = 39;
int Sensors::rainSensorDigitalPin = 25;
int Sensors::temperatureSensorPin = 27;
int Sensors::smallSoilMoistureAnalogSensor = 36;
int Sensors::smallSoilMoistureDigitalSensor = 16;
int Sensors::largeSoilMoistureAnalogSensor = 34;
int Sensors::largeSoilMoistureDigitalSensor = 14;
int Sensors::allSensorsActivationPin = 5;

int Sensors::lightValue = 0;
int Sensors::rainValue = 0;
bool Sensors::isRaining = 0;;
int Sensors::rainingThreshold = 0;
float Sensors::temperature = 0;
float Sensors::humidity = 0;
int Sensors::smallMoistureAnalogValue = 0;
bool Sensors::smallMoistureDigitalValue = 0;
int Sensors::largeMoistureAnalogValue = 0;
int Sensors::largeMoistureDigitalValue = 0;
bool Sensors::allSensorsAreActivated = false;

DHT dht(27, DHT11);

void Sensors::Initialize()
{
    pinMode(Sensors::lightSensorPin, INPUT);
    pinMode(Sensors::rainSensorAnalogPin, INPUT);
    pinMode(Sensors::rainSensorDigitalPin, INPUT);
    pinMode(Sensors::temperatureSensorPin, INPUT);
    pinMode(Sensors::smallSoilMoistureAnalogSensor, INPUT);
    pinMode(Sensors::smallSoilMoistureDigitalSensor, INPUT);
    pinMode(Sensors::largeSoilMoistureAnalogSensor, INPUT);
    pinMode(Sensors::largeSoilMoistureDigitalSensor, INPUT);
    pinMode(Sensors::allSensorsActivationPin, OUTPUT);
    digitalWrite(Sensors::allSensorsActivationPin, LOW);
    DHT _dht(temperatureSensorPin, DHT11);
    dht = _dht;
    dht.begin();
}

int Sensors::GetLightValue()
{
    Sensors::lightValue = analogRead(Sensors::lightSensorPin);
    return Sensors::lightValue;
}

int Sensors::GetRainAnalogValue()
{
    Sensors::rainValue = analogRead(Sensors::rainSensorAnalogPin);
    return Sensors::rainValue;
}
bool Sensors::GetRainDigitalValue()
{
    Sensors::isRaining = digitalRead(Sensors::rainSensorDigitalPin);
    return isRaining;
}

float Sensors::GetTemperature()
{
    Sensors::temperature = dht.readTemperature(false);
    return Sensors::temperature;
}

float Sensors::GetHumidity()
{
    Sensors::humidity = dht.readHumidity();
    return Sensors::humidity;
}

int Sensors::GetSmallSoilMoistureAnalogValue()
{
    Sensors::smallMoistureAnalogValue = analogRead(Sensors::smallSoilMoistureAnalogSensor);
    return Sensors::smallMoistureAnalogValue;
}

bool Sensors::GetSmallSoilMoistureDigitalValue()
{
    Sensors::smallMoistureDigitalValue = digitalRead(Sensors::smallSoilMoistureDigitalSensor);
    return Sensors::smallMoistureDigitalValue;
}

int Sensors::GetLargeSoilMoistureAnalogValue()
{
    Sensors::largeMoistureAnalogValue = analogRead(Sensors::largeSoilMoistureAnalogSensor);
    return Sensors::largeMoistureAnalogValue;
}

bool Sensors::GetLargeSoilMoistureDigitalValue()
{
    Sensors::largeMoistureDigitalValue = digitalRead(Sensors::largeSoilMoistureDigitalSensor);
    return Sensors::largeMoistureDigitalValue;
}

/// @brief this function is to turn on the sensor for a short period of time instead of always
void Sensors::CheckIfTurnOnSensors()
{
    if ((TimeManager::currentTime.seconds > 5 && TimeManager::currentTime.seconds < 10) 
        || (TimeManager::currentTime.seconds > 35 && TimeManager::currentTime.seconds < 40))
    {
        digitalWrite(Sensors::allSensorsActivationPin, HIGH);
        Sensors::allSensorsAreActivated = true;
    }
    else
    {
        if (Sensors::allSensorsAreActivated)
        {
            Sensors::allSensorsAreActivated = false;
            digitalWrite(Sensors::allSensorsActivationPin, LOW);
        }
    }
}