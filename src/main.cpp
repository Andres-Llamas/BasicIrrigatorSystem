#include "WifiServerManager.h"
#include "TimeManager.h"
#include <Arduino.h>
#include "Behaviors.h"

char* wifiName = "TP-Link_AP_72F2";
char* wifiPassword = "41168004";
TimeManager timeClock(-21600); // UTC offset calculated by  utc -06:00 mexico city = -6*60*60
Behaviors behaviors(D2); // the pin for the valve relay
WifiServerManager server(wifiName, wifiPassword, behaviors);

void ownFunction();

void setup()
{
  Serial.begin(9600);
  Serial.println("\nHello!");
  delay(1000);
  behaviors.Initialization();
  server.Initialize();
  server.StablishMDNSDirectionsAndBeginServer();
  timeClock.Initialization();
}

void loop()
{  
  server.UpdateServerCLient();
  timeClock.UpdateTime();  
  delay(500);    
}