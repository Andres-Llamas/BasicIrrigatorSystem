#ifndef Sensors_h
#define Sensors_h
#include <DHT.h>

class Sensors
{
private:
    /* data */
public:
    static int lightSensorPin;
    static int rainSensorAnalogPin;
    static int rainSensorDigitalPin;
    static int temperatureSensorPin;
    static int smallSoilMoistureAnalogSensor;
    static int smallSoilMoistureDigitalSensor;
    static int largeSoilMoistureAnalogSensor;
    static int largeSoilMoistureDigitalSensor;

    static int lightValue;
    static int rainValue;
    static bool isRaining;
    static int rainingThreshold;
    static float temperature;
    static float humidity;
    static int smallMoistureAnalogValue;
    static bool smallMoistureDigitalValue;
    static int largeMoistureAnalogValue;
    static int largeMoistureDigitalValue;
    static int allSensorsActivationPin;
    static bool allSensorsAreActivated;

    static void Initialize();
    static int GetLightValue();
    static int GetRainAnalogValue();
    static bool GetRainDigitalValue();
    static float GetTemperature();
    static float GetHumidity();
    static int GetSmallSoilMoistureAnalogValue();
    static bool GetSmallSoilMoistureDigitalValue();
    static int GetLargeSoilMoistureAnalogValue();
    static bool GetLargeSoilMoistureDigitalValue();
    static void CheckIfTurnOnSensors();
};

#endif