#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "WifiServerManager.h"
#include "Sensors.h"

char *_wifiName;
char *_wifiPassword;
WebServer _server(80);
Behaviors _behaviorsObject;

void handle_root();
void SetNumberOfTimersHandler();
void GetClockTimeFromListHandler();
void SetIrrigatorWithTimerHandler();
void CheckIrrigationValveStateHandler();

void WifiServerManager::Initialize()
{
    Serial.println("ESP starts");
    Serial.println(_wifiName);

    // Connect to your wi-fi modem
    WiFi.begin(_wifiName, _wifiPassword);

    // Check wi-fi is connected to wi-fi network
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected successfully");
    Serial.print("Got IP: ");
    Serial.println(WiFi.localIP()); // Show ESP32 IP on serial

    WifiServerManager::StablishMDNSDirectionsAndBeginServer();
    Serial.println("HTTP server started");
    delay(100);

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

    _server.on("/", handle_root);

    _server.on("/start", []()
               {_server.send(200, "text/plain", "Irrigation has started"); 
               _behaviorsObject.SetIrrigationActive(true); });
    _server.on("/stop", []()
               {_server.send(200, "text/plain", "Irrigation has stopped"); 
               _behaviorsObject.SetIrrigationActive(false); });

    _server.on("/check", CheckIrrigationValveStateHandler);
    _server.on("/timers", SetNumberOfTimersHandler);
    _server.on("/get", GetClockTimeFromListHandler);
    _server.on("/mode", SetIrrigatorWithTimerHandler);

    _server.begin();
}

void SetNumberOfTimersHandler()
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

void GetClockTimeFromListHandler()
{
    String index = "";
    if (_server.arg("index") == "")
        index = "Parameter not found";
    else
        index = _server.arg("index");
    int val = index.toInt();
    _behaviorsObject.GetClockTimeFromList(val);
}

void SetIrrigatorWithTimerHandler()
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

void CheckIrrigationValveStateHandler()
{
    _behaviorsObject.CheckIrrigationValveState();
    String message = _behaviorsObject.valveState;
    String HTML = "";
    HTML += "<!DOCTYPE html>";
    HTML += "<html>";
    HTML += "   <body>";
    HTML += "       <h1><center>" + message + "</center></h1>";    
    HTML += "   </body>";
    HTML += "</html>";
    _server.send(200, "text/html", HTML);
}

void WifiServerManager::UpdateServerCLient()
{
    _server.handleClient();
}

// Handle root url (/)
void handle_root()
{
    String HTML = "";
    HTML += "<!DOCTYPE html>";
    HTML += "<html>";
    HTML += "   <head>";
    HTML += "       <title>ESP Input Form</title>";
    HTML += "       <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
    HTML += "   </head>";
    HTML += "   <body>";
    HTML += "       <h1><center> Irrigator system </center></h1>";
    HTML += "       <a href=\"http://192.168.100.52/start\"><center>Start Irrigation</center></a>";
    HTML += "       <a href=\"http://192.168.100.52/stop\"><center>Stop Irrigation</center></a>";
    HTML += "       <a href=\"http://192.168.100.52/check\"><center>Check if valve is active</center></a>";
    HTML += "   <form action=\"/get\">";
    HTML += "       input1: <input type=\"text\" name=\"input1\">";
    HTML += "       <input type=\"submit\" value=\"Submit\">";
    HTML += "   </form><br>";
    HTML += "   </body>";
    HTML += "</html>";

    

    Serial.println("entered the login page");
    _server.send(200, "text/html", HTML);
}

WifiServerManager::WifiServerManager(char *wifiName, char *wifiPassword, Behaviors behaviorsObject)
{
    _wifiName = wifiName;
    _wifiPassword = wifiPassword;
    _behaviorsObject = behaviorsObject;
}