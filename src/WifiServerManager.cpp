#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "WifiServerManager.h"

char *_wifiName;
char *_wifiPassword;
WebServer _server(80);
Behaviors _behaviorsObject;

void WifiServerManager::Initialize()
{
    Serial.println("ESP starts");
    WiFi.mode(WIFI_STA);
    Serial.println("Try Connecting to ");
    Serial.println(_wifiName);
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

    if (MDNS.begin("riego"))
    {
        Serial.println("DNS started, available with: ");
        Serial.println("http://riego.local/");
        Serial.print("Or\nhttp://");        
        Serial.print(WiFi.localIP());
        Serial.print(".local/");
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
    _server.on("/check", []()
               {_server.send(200, "text/plain", "Irrigation has stopped"); 
               _behaviorsObject.CheckIrrigationValveState(); });
    _server.on("/timers", [&]()
               {_server.send(200, "text/plain", "Setting timers"); 
               WifiServerManager::SetNumberOfTimersHandler(); });
    _server.on("/get", [&]()
               {_server.send(200, "text/plain", "Getting timers"); 
               WifiServerManager::GetClockTimeFromListHandler(); });
    _server.on("/mode", [&]()
               {_server.send(200, "text/plain", "Setting irrigator activation mode: select 'manual' or 'timer'"); 
               WifiServerManager::SetIrrigatorWithTimerHandler(); });

    _server.begin();
}

void WifiServerManager::SetNumberOfTimersHandler()
{
    /*When the user enters "http://sisriego.local/timers", the user must write the parameters as.-
    http://sisriego.local/timers?startHour=1&startMinute=2&stopHour=3&stopMinute=4&indexToSet=5. or whit IP address
    http://192.168.100.23/timers?startHour=1&startMinute=2&stopHour=3&stopMinute=4&indexToSet=5
    then this method will count how many parameters there are, then will iterate for each one, if the name matches with the
    name of the variable from this method, then gets the value written from the user and stores it in a string, then after the
    iteration is done, this method will convert each string to int and store then uin the "clockTime" structure in order to pass that
    structure to the method "AddIrrigatorTimer" from "Behaviors.h"
    */
    String startHour = "";
    String startMinute = "";
    String stopHour = "";
    String stopMinute = "";
    String indexToSet = "";
    clockTime startTime;
    clockTime stopTime;
    String message = "Number of args received :";
    message += _server.args(); // Get number of parameters
    message += "\n";           // Add a new line

    for (int i = 0; i < _server.args(); i++)
    {

        message += "Arg nº" + (String)i + " -> "; // Include the current iteration value
        message += _server.argName(i) + ":";      // Get the name of the parameter
        message += _server.arg(i) + "\n";         // Get the value of the parameter
        if (_server.argName(i).equals("startHour"))
            startHour = _server.arg(i);
        else if (_server.argName(i).equals("startMinute"))
            startMinute = _server.arg(i);
        else if (_server.argName(i).equals("stopHour"))
            stopHour = _server.arg(i);
        else if (_server.argName(i).equals("stopMinute"))
            stopMinute = _server.arg(i);
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
    startTime.hours = startHour.toInt();
    startTime.minutes = startMinute.toInt();
    startTime.seconds = 0; // his is because I will use only hours and minutes to control the system
    stopTime.hours = stopHour.toInt();
    stopTime.minutes = stopMinute.toInt();
    stopTime.seconds = 0;
    _behaviorsObject.AddIrrigatorTimer(startTime, stopTime, indexToSet.toInt());
}

void WifiServerManager::GetClockTimeFromListHandler()
{
    String index = "";
    if (_server.arg("index") == "")
        index = "Parameter not found";
    else
        index = _server.arg("index");
    int val = index.toInt();
    _behaviorsObject.GetClockTimeFromList(val);
}

void WifiServerManager::SetIrrigatorWithTimerHandler()
{
    String state = "";
    if (_server.arg("mode") == "")
        state = "Parameter not found";
    else
        state = _server.arg("mode");

    if (state.equals("manual"))
        _behaviorsObject.SetIrrigatorWithTimer(false);
    else
        _behaviorsObject.SetIrrigatorWithTimer(true);
}

void WifiServerManager::UpdateServerCLient()
{
    _server.handleClient();
}

WifiServerManager::WifiServerManager(char *wifiName, char *wifiPassword, Behaviors behaviorsObject)
{
    _wifiName = wifiName;
    _wifiPassword = wifiPassword;
    _behaviorsObject = behaviorsObject;
}