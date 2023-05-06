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
    _server.on("/get", [&]()
               {_server.send(200, "text/plain", "Setting timers"); 
               WifiServerManager::GetClockTimeFromListHandler(); });

    _server.begin();
}

void WifiServerManager::SetNumberOfTimersHandler()
{
    /*When the user enters "http://sisriego.local/timers", the user must write the parameters as.-
    http://sisriego.local/timers?hour=val1&minute=val2&second=val3&index=val4
    then this method will count how many parameters there are, then will iterate for each one, if the name matches with the
    name of the variable from this method, then gets the value written from the user and stores it in a string, then after the
    iteration is done, this method will convert each string to int and store then uin the "clockTime" structure in order to pass that
    structure to the method "AddIrrigatorTimer" from "Behaviors.h"
    */
    String hour = "";
    String minute = "";
    String second = "";
    String message = "Number of args received :";
    String indexToSet = "";
    clockTime userTime;
    message += _server.args(); // Get number of parameters
    message += "\n";           // Add a new line

    for (int i = 0; i < _server.args(); i++)
    {

        message += "Arg nº" + (String)i + " -> "; // Include the current iteration value
        message += _server.argName(i) + ":";      // Get the name of the parameter
        message += _server.arg(i) + "\n";         // Get the value of the parameter
        if (_server.argName(i).equals("hour"))
            hour = _server.arg(i);
        else if (_server.argName(i).equals("minute"))
            minute = _server.arg(i);
        else if (_server.argName(i).equals("second"))
            second = _server.arg(i);
        else if (_server.argName(i).equals("index"))
            indexToSet = _server.arg(i);
        else
        {
            message = "Error at ";
            message += i;
            message += " parameter index. The parameter is incorrect";
        }
    }
    _server.send(200, "text/plain", message); // Response to the HTTP request
    userTime.hours = hour.toInt();
    userTime.minutes = minute.toInt();
    userTime.seconds = second.toInt();
    _behaviorsObject.AddIrrigatorTimer(userTime, indexToSet.toInt());
}

void WifiServerManager::GetClockTimeFromListHandler()
{
    String index = "";
    if(_server.arg("index") == "")
        index = "Parameter not found";
    else
        index = _server.arg("index");
    int val = index.toInt();
    _behaviorsObject.GetClockTimeFromList(val);
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