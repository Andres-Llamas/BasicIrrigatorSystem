#include "WifiServerManager.h"
#include "TimeManager.h"
#include <Arduino.h>
#include "Behaviors.h"
#include "Sensors.h"

char *wifiName = "TP-Link_AP_72F2"; //"TP-Link_AP_72F2";
char *wifiPassword = "41168004";    //"41168004";
int lightSensor = 35;
int rainSensorDigital = 25;
int rainSensorAnalog = 39;
int temperatureSensor = 27;
int smallSoilMostureAnalogPin = 36;
int smallSoilMostureDigitalgPin = 16;
int largeSoilMostureAnalogPin = 34;
int largeSoilMostureDigitalgPin = 14;
int relayPin = 17;
int turnAllSensorsOnOffPin = 5;

Behaviors behaviors(relayPin); // the pin for the valve relay
WifiServerManager server(wifiName, wifiPassword, behaviors);

void setup()
{
  Serial.begin(9600);
  Serial.println("\nHello!");
  pinMode(turnAllSensorsOnOffPin, OUTPUT);
  Sensors::lightSensorPin = lightSensor;
  Sensors::rainSensorAnalogPin = rainSensorAnalog;
  Sensors::rainSensorDigitalPin = rainSensorDigital;
  Sensors::smallSoilMoistureAnalogSensor = smallSoilMostureAnalogPin;
  Sensors::smallSoilMoistureDigitalSensor = smallSoilMostureDigitalgPin;
  Sensors::largeSoilMoistureAnalogSensor = largeSoilMostureAnalogPin;
  Sensors::largeSoilMoistureDigitalSensor = largeSoilMostureDigitalgPin;
  Sensors::allSensorsActivationPin = turnAllSensorsOnOffPin;
  Sensors::Initialize();
  delay(1000);
  behaviors.startOnlyWhenIsNotRaining = true;
  behaviors.Initialization();
  server.Initialize();
  server.StablishMDNSDirectionsAndBeginServer();
  TimeManager::Initialization();
}

void loop()
{
  server.UpdateServerCLient();
  TimeManager::UpdateTime();
  behaviors.CheckIfCanActivateValve();
  Sensors::CheckIfTurnOnSensors();

  if (Sensors::allSensorsAreActivated)
  {
    Sensors::GetLightValue();
    Sensors::GetTemperature();
    Sensors::GetHumidity();
    Sensors::GetRainAnalogValue();
    Sensors::GetRainDigitalValue();
    Sensors::GetLargeSoilMoistureAnalogValue();
    Sensors::GetLargeSoilMoistureDigitalValue();
    Serial.print(" |||| light value: ");
    Serial.print(Sensors::lightValue);
    Serial.print(" | temperature: ");
    Serial.print(Sensors::temperature);
    Serial.print(" | Humidity: ");
    Serial.print(Sensors::humidity);
    Serial.print(" | rain value: ");
    Serial.print(Sensors::rainValue);
    Serial.print(" | is raining?: ");
    Serial.print(Sensors::isRaining);
    Serial.print(" | soil moisture: ");
    Serial.print(Sensors::largeMoistureAnalogValue);
    Serial.print(" | is soil wet?: ");
    Serial.println(Sensors::largeMoistureDigitalValue);
    Serial.print(TimeManager::currentTime.seconds);
  }
  delay(200);
}