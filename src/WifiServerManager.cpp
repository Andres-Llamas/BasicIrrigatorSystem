#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "WifiServerManager.h"
#include "Sensors.h"
#include "CustomStructs.h"

char *_wifiName;
char *_wifiPassword;
WebServer _server(80);
Behaviors _behaviorsObject;

void handle_root();
void SetIrrigationActiveHandler();
void SetIrrigationInactiveHandler();
void SetNumberOfTimersHandler();
void GetClockTimeFromListHandler();
void SetIrrigatorWithTimerHandler();
void CheckIrrigationValveStateHandler();
void CheckSensorsHandler();

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
    _server.on("/start", SetIrrigationActiveHandler);
    _server.on("/stop", SetIrrigationInactiveHandler);
    _server.on("/check", CheckIrrigationValveStateHandler);
    _server.on("/timers", SetNumberOfTimersHandler);
    _server.on("/get", GetClockTimeFromListHandler);
    _server.on("/mode", SetIrrigatorWithTimerHandler);
    _server.on("/sensors", CheckSensorsHandler);

    _server.begin();
}
void SetIrrigationActiveHandler()
{
    String HTML = "";
    HTML += "<!DOCTYPE html>";
    HTML += "<html>";
    HTML += "   <body>";
    HTML += "       <h1><center>Irrigation started</center></h1>";
    HTML += "       <a href=\"http://"; HTML += WiFi.localIP().toString(); HTML += "/\"><center>Return</center></a>";
    HTML += "   </body>";
    HTML += "</html>";    
    _behaviorsObject.SetIrrigationActive(true);
    _server.send(200, "text/html", HTML);
}
void SetIrrigationInactiveHandler()
{
    String HTML = "";
    HTML += "<!DOCTYPE html>";
    HTML += "<html>";
    HTML += "   <body>";
    HTML += "       <h1><center>Irrigation stoped</center></h1>";
    HTML += "       <a href=\"http://"; HTML += WiFi.localIP().toString(); HTML += "/\"><center>Return</center></a>";
    HTML += "   </body>";
    HTML += "</html>";    
    _behaviorsObject.SetIrrigationActive(false);
    _server.send(200, "text/html", HTML);
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
        else if (_server.argName(i).equals("indexToSet"))
            indexToSet = _server.arg(i);
        else
        {
            message = "Error at ";
            message += i;
            message += " parameter index. The parameter is incorrect";
        }
    }
    startTime.hours = startHour.toInt();
    startTime.minutes = startMinute.toInt();
    startTime.seconds = 0; // his is because I will use only hours and minutes to control the system
    stopTime.hours = stopHour.toInt();
    stopTime.minutes = stopMinute.toInt();
    stopTime.seconds = 0;
    _behaviorsObject.AddIrrigatorTimer(startTime, stopTime, indexToSet.toInt());
    _server.send(200, "text/plain", message); // Response to the HTTP request
}

void GetClockTimeFromListHandler()
{
    String index = "";
    if (_server.arg("index") == "")
        index = "Parameter not found";
    else
        index = _server.arg("index");
    int val = index.toInt();
    clockTime time = _behaviorsObject.GetClockTimeFromList(val);
    String HTML = "";
    HTML += "<!DOCTYPE html>";
    HTML += "<html>";
    HTML += "   <body>";
    HTML += "       <h1><center>Valve activation time from timer number " + index + "</center></h1>";
    HTML += "       <h2><center>Hour: ";
    HTML += time.hours;
    HTML += "</center></h2>";
    HTML += "       <h2><center>Minutes: ";
    HTML += time.minutes;
    HTML += "</center></h2>";
    HTML += "       <h2><center>Seconds: ";
    HTML += time.seconds;
    HTML += "</center></h2>";
    HTML += "       <a href=\"http://"; HTML += WiFi.localIP().toString(); HTML += "/\"><center>Return</center></a>";
    HTML += "   </body>";
    HTML += "</html>";
    _server.send(200, "text/html", HTML);
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
    String HTML = "";
    HTML += "<!DOCTYPE html>";
    HTML += "<html>";
    HTML += "   <body>";
    HTML += "       <h1><center> changed irrigation to " + state + "</center></h1>";
    HTML += "       <a href=\"http://"; HTML += WiFi.localIP().toString(); HTML += "/\"><center>Return</center></a>";
    HTML += "   </body>";
    HTML += "</html>";
    _server.send(200, "text/html", HTML);
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
    HTML += "       <a href=\"http://"; HTML += WiFi.localIP().toString(); HTML += "/\"><center>Return</center></a>";
    HTML += "   </body>";
    HTML += "</html>";
    _server.send(200, "text/html", HTML);
}

void CheckSensorsHandler()
{
    
    String HTML = "";
    HTML += "<!DOCTYPE html>";
    HTML += "<html>";
    HTML += "   <body>";
    HTML += "       <h1><center>Sensors values</center></h1>";
    HTML += "       <p><center> Light amount = ";HTML += Sensors::lightValue; HTML += "</center></p>";
    HTML += "       <p><center> Temperature = ";HTML += Sensors::temperature; HTML += "c*</center></p>";
    HTML += "       <p><center> Humidity = ";HTML += Sensors::humidity; HTML += "</center></p>";
    HTML += "       <p><center> Is raining? = ";HTML += Sensors::isRaining? "false" : "true"; HTML += "</center></p>";
    HTML += "       <p><center> Raining value = ";HTML += Sensors::rainValue; HTML += "</center></p>";
    HTML += "       <p><center> Soil moisture number 1 = ";HTML += Sensors::smallMoistureAnalogValue; HTML += "</center></p>";
    HTML += "       <p><center> Is soil number 1 wet? = ";HTML += Sensors::smallMoistureDigitalValue? "nop" : "yes is wet"; HTML += "</center></p>";
    HTML += "       <p><center> Soil moisture number 2 = ";HTML += Sensors::largeMoistureAnalogValue; HTML += "</center></p>";
    HTML += "       <p><center> Is soil number 2 wet? = ";HTML += Sensors::largeMoistureAnalogValue? "nop" : "yes is wet"; HTML += "</center></p>";
    HTML += "       <a href=\"http://"; HTML += WiFi.localIP().toString(); HTML += "/\"><center>Return</center></a>";
    HTML += "   </body>";
    HTML += "</html>";
    _server.send(200, "text/html", HTML);
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
    HTML += "       <a href=\"http://"; HTML += WiFi.localIP().toString(); HTML += "/start\"><center>Start Irrigation</center></a>";
    HTML += "       <a href=\"http://"; HTML += WiFi.localIP().toString(); HTML += "/stop\"><center>Stop Irrigation</center></a>";
    HTML += "       <a href=\"http://";HTML += WiFi.localIP().toString(); HTML +="/check\"><center>Check if valve is active</center></a>";
    HTML += "       <a href=\"http://";HTML += WiFi.localIP().toString(); HTML +="/sensors\"><center>Check Sensors values</center></a>";
    HTML += "       <p><center> To set the irrigator mode to manual or automatic (with timers) is with: http://"; HTML += WiFi.localIP().toString(); HTML += "/mode?mode=automatic/manual</center></p>";
    HTML += "       <p><center> To set a timer is with: http://"; HTML += WiFi.localIP().toString(); HTML += "/timers?startHour=1&startMinute=2&stopHour=3&stopMinute=4&indexToSet=5</center></p>";
    HTML += "       <p><center> To check a timer is with: http://"; HTML += WiFi.localIP().toString(); HTML += "/get?index=0</center></p>";
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

void WifiServerManager::UpdateServerCLient()
{
    _server.handleClient();
}