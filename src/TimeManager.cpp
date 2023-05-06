#include "TimeManager.h"
#include <NTPClient.h>
#include "WiFiUdp.h"

long _utcOffsetInSeconds = -21600; // The offset difference between the Time zones calculated by UTC*60*60
char *daysOfTheWeek[7] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", _utcOffsetInSeconds);

void TimeManager::Initialization()
{
    timeClient.begin();
}

void TimeManager::UpdateTime()
{
    timeClient.update();
    TimeManager::day = daysOfTheWeek[timeClient.getDay()];
    TimeManager::hours = timeClient.getHours();
    TimeManager::minutes = timeClient.getMinutes();
    TimeManager::seconds = timeClient.getSeconds();
}
void TimeManager::ShowDateAndTime()
{
    Serial.print(TimeManager::day);
    Serial.print(", ");
    Serial.print(TimeManager::hours);
    Serial.print(":");
    Serial.print(TimeManager::minutes);
    Serial.print(":");
    Serial.println(TimeManager::seconds);
}

TimeManager::TimeManager(long utcOffsetInSeconds)
{
    _utcOffsetInSeconds = utcOffsetInSeconds;
}