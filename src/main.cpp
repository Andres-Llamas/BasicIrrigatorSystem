#include "WifiServerManager.h"
#include "TimeManager.h"
#include <Arduino.h>
#include "Behaviors.h"

char *wifiName = "TP-Link_AP_72F2";   //"TP-Link_AP_72F2";
char *wifiPassword = "41168004" ;               //"41168004";
Behaviors behaviors(12); // the pin for the valve relay
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
  TimeManager::Initialization();
}

void loop()
{  
  server.UpdateServerCLient();
  TimeManager::UpdateTime();
  behaviors.CheckTimeForValveActivation();
  delay(500);    
}