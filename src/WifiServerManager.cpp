#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "WifiServerManager.h"

char *_wifiName;
char *_wifiPassword;
ESP8266WebServer _server(80);
Behaviors _behaviorsObject;

void WifiServerManager::Initialize()
{
    Serial.println("ESP starts");
    WiFi.mode(WIFI_STA);
    Serial.println("Initiating WIFI connection with " + *_wifiName);
    WiFi.begin(_wifiName, _wifiPassword);

    Serial.print("Connecting...");

    while (WiFi.status() != WL_CONNECTED)
    { // Loop which makes a point every 500ms until the connection process has finished

        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected! IP-Address: ");
    Serial.println(WiFi.localIP()); // Displaying the IP Address

    if (MDNS.begin("sisriego"))
    {
        Serial.println("DNS started, available with: ");
        Serial.println("http://sisriego.local/");
    }
    else
        Serial.println("Error starting MDNS");
}

void WifiServerManager::StablishMDNSDirectionsAndBeginServer()
{
    _server.onNotFound([]()
                       { _server.send(404, "text/plain", "Link was not found!"); });

    _server.on("/", []()
               { _server.send(200, "text/plain", "Welcome motherfucker!!!"); });

    _server.on("/start", []()
               {_server.send(200, "text/plain", "Irrigation has started"); 
               _behaviorsObject.SetIrrigationActive(true); });
    _server.on("/stop", []()
               {_server.send(200, "text/plain", "Irrigation has stopped"); 
               _behaviorsObject.SetIrrigationActive(false); });
    _server.on("/timers", [&]()
               {_server.send(200, "text/plain", "Setting timers"); 
               WifiServerManager::SetNumberOfTimersHandler(); });

    _server.begin();
}

void WifiServerManager::SetNumberOfTimersHandler()
{

    String message = ""; // this is where we will store the values of the parameters;

    if (_server.arg("numTimers") == "")
    { // Parameter not found

        message = "numTimers Argument not found";
    }
    else
    { // Parameter found        
        message += _server.arg("numTimers"); // Gets the value of the query parameter
    }

    _server.send(404, "text / plain", "numTimers Argument = " + message); // Returns the HTTP response
    _behaviorsObject.SetNumberOfTimers(message.toInt());
}

void WifiServerManager::UpdateServerCLient()
{
    _server.handleClient();
    MDNS.update();
}

WifiServerManager::WifiServerManager(char *wifiName, char *wifiPassword, Behaviors behaviorsObject)
{
    _wifiName = wifiName;
    _wifiPassword = wifiPassword;
    _behaviorsObject = behaviorsObject;
}