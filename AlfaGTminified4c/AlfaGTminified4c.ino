/**
* NEOPIXEL KITT- minified.
* v4c
*  no dht
*   ESP8266WebServer TESTER
* **/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
/*
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ThingSpeak.h>
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
// NEW TOUCHES...
#include <ESP8266HTTPUpdateServer.h>
#include <FS.h>

// AND..
const char *host = "alfagt";
ESP8266HTTPUpdateServer httpUpdater;
boolean rolling, connd = false;
uint8_t rgb[] = {0, 0, 200};
byte ranPixs = 0;

#include "index.h"
#include "kittcmd.h"

bool runningNOW=false;
MDNSResponder mdns;
ESP8266WebServer server(80);
WiFiClient client;
#define HOST_NAME "alfagt"

//String colour = "red";String mode = "kitt";int theSpeed = 25;int cycles = 5;int width = 4;
int stdDelaySec = 10;
int watsdoin=1;
const char ssid[] = "TPG 15EA";const char pass[] = "abcd1989";
const char ssid3[] = "AlfaRomeoGT";const char pass3[] = "turismoGT";
const char ssid2[] = "AndroidAP";const char pass2[] = "tttttttt";
//AP-STA
const char *ssAPid = "AlfaRomeo";const char *passAP = "turismoGT";
long lastTime1, lastTime2, lastTime3, lastTry, nextRun = 0;
float alfaTemp, alfaHumid = 10;
/*
minified...

#define DHTPIN 0 //// which is D6... or was it 14 (D5)?
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
//unsigned long myChannelNumber = 404585;  key-W124WS7UN76VCASZ
unsigned long myChannelNumber = 511863;
const char *myWriteAPIKey = "ZPH42LNUJXJHUW0O";
char alfaTempC[6];
char alfaHumidC[6];
int delField = 4;
int kittField = 3;
int aTfield = 1;
int aHfield = 2;
int numRanfield = 5;

void getTemperature()
{
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println("Error reading temperature!");
  }
  else
  {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    alfaTemp = event.temperature;
    Serial.println(" *C");
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println("Error reading humidity!");
  }
  else
  {
    //Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    alfaHumid = event.relative_humidity;
    Serial.println("%");
  }
  //dtostrf(alfaTemp, 2, 2 , alfaTempS);
  //dtostrf(alfaHumid, 2, 2 , alfahumidS);
  ThingSpeak.setField(aTfield, alfaTemp);
  ThingSpeak.setField(aHfield, alfaHumid);
  ThingSpeak.setField(kittField, watsdoin);
  ThingSpeak.setField(delField, String(stdDelaySec));
  ThingSpeak.setField(numRanfield, ranPixs);
  ThingSpeak.setField(8, String(millis()));
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  Serial.println("written to thingSpeak");
  delay(200);
}

*/

void handNext()
{
  server.send(200, "text/plain", String(nextRun));
}
void handTemp()
{
  server.send(200, "text/plain", String(alfaTemp));
}
void handHumid()
{
  server.send(200, "text/plain", String(alfaHumid));
}
void handWd()
{
  if (!server.hasArg("watsdoin"))
    server.send(200, "text/plain", String(watsdoin));
  String tt = server.arg("watsdoin");
  watsdoin = tt.toInt();
  server.send(200, "text/plain", String(watsdoin));
}
void handDelay()
{
   if (!server.hasArg("delay"))
    server.send(200, "text/plain", String(stdDelaySec));
  String LEDvalue="";
  LEDvalue = server.arg("delay");
  stdDelaySec = LEDvalue.toInt();
  server.send(200, "text/plain", String(stdDelaySec));
}
void handleReset()
{
  delay(1000);
  ESP.restart();
}

boolean setServices()
{
    MDNS.begin(host);
    ArduinoOTA.setHostname("alfagt");
    ArduinoOTA.onStart([]() {
        watsdoin = 0;
        Serial.println("starting");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
            Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
            Serial.println("End Failed");
    });
    ArduinoOTA.begin();
    server.on("/stats.json", stats);
    server.on("/numran", handRan);
    server.on("/rgb", cmd);
    server.on("/delay", handDel);
    server.on("/watsdoin", handWd);
    server.on("/roll", handRoll);
    server.serveStatic("/js", SPIFFS, "/js");
    server.serveStatic("/css", SPIFFS, "/css");
    server.serveStatic("/img", SPIFFS, "/img");
    /*HTTP_POST, []() {    updateGpio();});*/
    server.serveStatic("/", SPIFFS, "/index.html");
    httpUpdater.setup(&server);
    server.begin();
    MDNS.addService("http", "tcp", 80);
    return true;
}

boolean tryToConnect()
{
    long startConn = millis();
    String hostname(HOSTNAME);
    WiFi.hostname(hostname);
    WiFi.mode(WIFI_AP_STA);
    Serial.print(". ");
    WiFi.begin(ssid, pass);
    Serial.print(". ");
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.print(". ");
        delay(20);
        WiFi.begin(ssid, pass);
        Serial.print(". ");
        long diff = (millis() - startConn);
        if (diff >= 3000)
        {
            Serial.println("WiFi failed, next...");
            WiFi.begin(ssid2, pass2);
            Serial.print(". ");
        }
        while (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            Serial.print(". ");
            WiFi.begin(ssid2, pass2);
            delay(20);
            Serial.println(". ");
            long diff2 = (millis() - (startConn + diff));
            if (diff2 >= 3000)
            {
                Serial.println("WiFi failed, third time lucky?");
                Serial.print(". ");
                WiFi.begin(ssid3, pass3);
                Serial.print(". ");
                while (WiFi.waitForConnectResult() != WL_CONNECTED)
                {
                    Serial.print(". ");
                    delay(20);
                    WiFi.begin(ssid3, pass3);
                    Serial.print(". ");
                    long diff = (millis() - startConn);
                    if (diff >= 3000)
                    {
                        Serial.println("WiFi failed, third time lucky?");
                        return false;
                    }
        }
        Serial.print("CONNECTED TO ");
        Serial.println(ssid2);
        bool servs = setServices();
        if (servs)
        {
            return true;
        }
        else
        {
            Serial.println("error setting up services?");
            return true;
        }
    }
    Serial.print("CONNECTED TO ");
    Serial.println(ssid);
    bool servs = setServices();
    if (servs)
    {
        return true;
    }
    else
    {
        Serial.println("error setting up services?");
        return true;
    }
}

void runThePix()
{
  runningNOW=true;
  ranPixs++;
  Serial.println("runnin pix");
  if (watsdoin == 0)
  {
    Serial.println("off");
  }
  else if (watsdoin == 1)
  {
    knightRider(5, 40, 4, 0xFF1000); // (original orange-red)
   }
  else if (watsdoin == 7)
  {
    kitt();
  }
  else if (watsdoin == 4)
  {
    copMode();
  }
  else if (watsdoin == 2)
  {
    knightRider(5, 25, 4, 0xFFFF00); // yellow
  }
  else if (watsdoin == 5)
  {
    knightRider(5, 25, 4, 0x00FF00); // green
  }
  else if (watsdoin == 8)
  {
    knightRider(5, 25, 4, 0x0000FF); // blue
  }
  else if (watsdoin == 10)
  {
    theaterChase(5, 10);
  }
  else if (watsdoin == 11)
  {
    theaterChaseRainbow(5);
  }
   else if (watsdoin == 14)
  {
   copMode2(stdDelaySec);
  }
  else if (watsdoin == 12)
  {
    colorWipe(60, 5); // yellowish?
  }
  else if (watsdoin == 15)
  {
    colorWipe(100, 5);
  }
  else if (watsdoin == 18)
  {
    colorWipe(170, 5); // blue
  }
    else if (watsdoin == 19)
  {
  whiteOverRainbow(25,75,8);
    }
    else if (watsdoin == 17)
    {
  pulseWhite(45);
    }
  else if (watsdoin == 16)
  {
  rainbowFade2White(10,5,2);
 }
 Serial.println("done pix");
}

void handleRoot()
{
  if (server.hasArg("nextrun"))
  {
    handNext();
  }
  if (server.hasArg("temp"))
  {
    handTemp();
  }
  if (server.hasArg("humid"))
  {
    handHumid();
  }
  if (server.hasArg("watsdoin"))
  {
    handWd();
  }
  if (server.hasArg("delay"))
  {
    handDelay();
  }
  else
  {
    String s = MAIN_page;
    server.send(200, "text/html", s);
  }
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}




void setup(void)
{
 /// pinMode(BUILTIN_LED, OUTPUT);
 /// digitalWrite(BUILTIN_LED, HIGH);
  strip.begin();
  strip.show();
  colorWipe(150,5);   
  Serial.begin(115200);
  Serial.println("STARTING");
  stdDelaySec = 30;
  firstRun = true;
  watsdoin = 1;
  String htname = "alfagt.local";
  String hostname(htname);
  WiFi.hostname(hostname);
  Serial.println("Connecting Wifi...");
 
  connd=tryToConnect(ssid3, pass3);
  delay(50);
  if(!connd)
  {
  connd=tryToConnect(ssid2, pass2); 
  delay(50); 
  }
  if(!connd)
  {
  connd=tryToConnect(ssid, pass);  
  delay(50); 
  }
  if(!connd)
  {
  delay(5000);
  ///WiFi.mode(WIFI_AP);
  WiFi.softAP(ssAPid,passAP);
  //ESP.restart();
  }
  digitalWrite(BUILTIN_LED, LOW);
  ArduinoOTA.setPort(8777);
	ArduinoOTA.setHostname("alfagt");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });

  server.on("/", handleRoot);
  server.on("/reset", handleReset);
  server.on("/nextrun", handNext);
  server.on("/delay", handDelay);
  server.on("/watsdoin", handWd);
  server.on("/temp", handTemp);
  server.on("/humid", handHumid);
  server.onNotFound(handleNotFound);
  server.begin();
  ArduinoOTA.begin();
  if (MDNS.begin("alfagt"))
  {
    Serial.println("MDNS responder online");
  }
  MDNS.addService("http", "tcp", 80);

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("Temperature");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" *C");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" *C");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" *C");
  dht.humidity().getSensor(&sensor);
  Serial.println("Humidity");
  Serial.print("Sensor:     ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  delay(100);
  ThingSpeak.begin(client);
}

void loop(void)
{
  millis() = millis();
  ArduinoOTA.handle();
 
  server.handleClient();
  if (firstRun)
  {
    firstRun = false;
    newLoop();
  }
  if (!connd){
  if(millis() >= (lastTry + 60000)){
  WiFi.mode(WIFI_STA);
  lastTry = millis();
  connd=tryToConnect(ssid3, pass3);
  delay(250);
  if(!connd)
  {
  connd=tryToConnect(ssid2, pass2); 
  delay(250); 
  if(!connd)
  {
  connd=tryToConnect(ssid, pass);  
  delay(250); 
  if(!connd)
  {
  delay(250);
  WiFi.mode(WIFI_AP);
  WiFi.begin(ssAPid,passAP);
    }
}
 }
}
  }

  if (millis() >= (lastTime3 + 10000))
  {
    lastTime3=millis();
	long lastRun = (millis()-lastTime1);
    Serial.print("last1 was ");
    Serial.println(lastRun);
      nextRun = ((stdDelaySec * 1000)-lastRun);
      Serial.print("nextrun=");
   Serial.println(nextRun);
   }

  if (millis() >= (lastTime1 + (stdDelaySec * 1000)))
  {
    if (!runningNOW){
    runThePix();
    runningNOW=false;
    lastTime1 = millis();
} else {
    Serial.println("one pixelrun skipped");
    lastTime1 = millis();
  }
 }
  if (millis() >= (lastTime2 + (90 * 1000)))
  {
    getTemperature();
    lastTime2 = millis();
    }
  yield();

}
/**
* NEOPIXEL KITT BETA - PRODUCTION
* v8a - espserver
*  dht
* thinkspeak
* **/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include "index.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

bool runningNOW=false;
MDNSResponder mdns;
ESP8266WebServer server(80);
WiFiClient client;
#define HOST_NAME "alfagt"
#include "kittcmd.h"

//String colour = "red";String mode = "kitt";int theSpeed = 25;int cycles = 5;int width = 4;
int stdDelaySec = 10;
int ranPixs=5;    // num of mins (when not connd) to wait in-between a reset...
int watsdoin=1;
const char ssid[] = "TPG 15EA";
const char pass[] = "abcd1989";

const char ssid3[] = "AlfaRomeoGT";
const char pass3[] = "turismoGT";
const char ssid2[] = "AndroidAP";
const char pass2[] = "tttttttt";
//AP-STA
const char *ssAPid = "AlfaRomeo";
const char *passAP = "turismoGT";
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway2(192, 168, 43, 1);
IPAddress ip2(192, 168, 43, 34);
long millis(), lastTime1, lastTime2, lastTime3, lastTry, nextRun = 0;
bool connd, firstRun, secondRun, boutRunPix, wifConnected = false;

float alfaTemp = 10;
float alfaHumid = 10;
/*
minified...

#define DHTPIN 0 //// which is D6... or was it 14 (D5)?
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
//unsigned long myChannelNumber = 404585;  key-W124WS7UN76VCASZ
unsigned long myChannelNumber = 511863;
const char *myWriteAPIKey = "ZPH42LNUJXJHUW0O";
char alfaTempC[6];
char alfaHumidC[6];
int delField = 4;
int kittField = 3;
int aTfield = 1;
int aHfield = 2;
int numRanfield = 5;


void getTemperature()
{
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature))
  {
    Serial.println("Error reading temperature!");
  }
  else
  {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    alfaTemp = event.temperature;
    Serial.println(" *C");
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity))
  {
    Serial.println("Error reading humidity!");
  }
  else
  {
    //Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    alfaHumid = event.relative_humidity;
    Serial.println("%");
  }
  //dtostrf(alfaTemp, 2, 2 , alfaTempS);
  //dtostrf(alfaHumid, 2, 2 , alfahumidS);
  ThingSpeak.setField(aTfield, alfaTemp);
  ThingSpeak.setField(aHfield, alfaHumid);
  ThingSpeak.setField(kittField, watsdoin);
  ThingSpeak.setField(delField, String(stdDelaySec));
  ThingSpeak.setField(numRanfield, ranPixs);
  ThingSpeak.setField(8, String(millis()));
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  Serial.println("written to thingSpeak");
  delay(200);
}



void handNext()
{
  server.send(200, "text/plain", String(nextRun));
}
void handTemp()
{
  server.send(200, "text/plain", String(alfaTemp));
}
void handHumid()
{
  server.send(200, "text/plain", String(alfaHumid));
}
void handWd()
{
  if (!server.hasArg("watsdoin"))
    server.send(200, "text/plain", String(watsdoin));
  String tt = server.arg("watsdoin");
  watsdoin = tt.toInt();
  server.send(200, "text/plain", String(watsdoin));
}
void handDelay()
{
   if (!server.hasArg("delay"))
    server.send(200, "text/plain", String(stdDelaySec));
  String LEDvalue="";
  LEDvalue = server.arg("delay");
  stdDelaySec = LEDvalue.toInt();
  server.send(200, "text/plain", String(stdDelaySec));
}
void handleReset()
{
  delay(1000);
  ESP.restart();
}

*/



void runThePix()
{
    long the = millis();
    ranPixs++;
    Serial.print("runnin");
    Serial.println(watsdoin);
    int doit = watsdoin;
    if (rolling)
    {
        watsdoin++;
        if (watsdoin == 16)
        {
            watsdoin = 1;
        }
    }
    if (doit == 0)
    {
        Serial.println("off");
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    // STARTUP..
    else if (doit == 99)
    {
        watsdoin--;
        first();
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 98)
    {
        watsdoin--;
        then1();
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 97)
    {
        watsdoin--;
        then2();
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 96)
    {
        watsdoin = 1;
        rolling = true;
        theaterChaseRainbow(15);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    // DEFAULT...
    else if (doit == 1)
    {
        knightRider(5, 35, 3, 0xFF1000);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    // WEB COLOUR...
    else if (doit == 22)
    {
        uint32_t col = rgb2hex(rgb[0], rgb[1], rgb[2]);
        knightRider(5, 25, 3, col);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    // THE ROLLOUT...
    else if (doit == 2)
    {
        kitt();
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 3)
    {
        theaterChase(strip.Color(0, 0, 255), 20);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 4)
    {
        rainbowBetter(20);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 5)
    {
        rainbowCycle(25);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 6)
    {
        theaterChase(strip.Color(150, 0, 0), 20);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 7)
    {
        rainbow();
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 8)
    {
        theaterChase(strip.Color(0, 0, 200), 20);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 9)
    {
        chaseMe(150);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 10)
    {
        theaterChase(strip.Color(0, 200, 0), 30);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 11)
    {
        theaterChaseRainbow(150);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 12)
    {
        pulseWhite(45);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 13)
    {
        rainbowFade2White(10, 5, 2);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 14)
    {
        whiteOverRainbow(25, 75, 8);
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 15)
    {
        fullWhite();
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 66)
    {
        copMode();
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    else if (doit == 67)
    {
        copMode2();
        long diff = (the - millis());
        Serial.println(diff);
        return;
    }
    Serial.println("done pix");
}

void handleArgs()
{
    for (uint8_t i = 0; i < server.args(); i++)
    {
        String argu = server.argName(i);
        String argVal = server.arg(i);
    if (argu == "watsdoin")
    {
    watsdoin=argVal.toInt();
    }
    else if (argu == "rolling")
    {
    rolling = argVal.toInt();
    
    }
    else if (argu == "red")
    {
        rgb[0] = argVal.toInt();
    }
else if (argu == "grn")
    {
        rgb[1] = argVal.toInt();
    }
    else if (argu == "blu")
    {
        rgb[2] = argVal.toInt();
    }
    }

void handleRoot(){
    String s = MAIN_page;
    server.send(200, "text/html", s);
  }

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  server.send(404, "text/plain", message);
}




void setup(void)
{
 strip.begin();
  strip.show();
  colorWipe(150,5);   
  Serial.begin(115200);
  Serial.println("STARTING");
  
  SPIFFS.begin();
  Serial.println("File System Initialized");
  
  String htname = "alfagt.local";
  String hostname(htname);
  WiFi.hostname(hostname);
  watsdoin = 99;
  rolling = false;
  connd = tryToConnect();
  if (connd)
  {
      theaterChase(green(200), 5);
  }
  else
  {
      theaterChase(red(200), 5);
  }
/*
  ArduinoOTA.setPort(8777);
	ArduinoOTA.setHostname("alfagt");
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });

  server.on("/", handleRoot);
  server.on("/reset", handleReset);
  server.on("/nextrun", handNext);
  server.on("/delay", handDelay);
  server.on("/watsdoin", handWd);
  server.on("/temp", handTemp);
  server.on("/humid", handHumid);
  server.onNotFound(handleNotFound);
  server.begin();
  ArduinoOTA.begin();
  if (MDNS.begin("alfagt"))
  {
    Serial.println("MDNS responder online");
  }
  MDNS.addService("http", "tcp", 80);

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println("Temperature");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" *C");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" *C");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" *C");
  dht.humidity().getSensor(&sensor);
  Serial.println("Humidity");
  Serial.print("Sensor:     ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  delay(100);
  ThingSpeak.begin(client);
  */
 
}

void loop(void)
{
  ArduinoOTA.handle();
  server.handleClient();
  
  if (!connd){
    if(millis() >= (lastTry + 60000)){
        lastTry = millis();
        connd=tryToConnect();
        delay(250);
    }
}

  if (millis() >= (lastTime3 + 10000))
  {
    lastTime3=millis();
	long lastRun = (millis()-lastTime1);
    nextRun = ((stdDelaySec * 1000)-lastRun);
    }

  if (millis() >= (lastTime1 + (stdDelaySec * 1000)))
  {
    if (!runningNOW){
    runThePix();
    runningNOW=false;
    lastTime1 = millis();
} else {
    Serial.println("one pixelrun skipped");
    lastTime1 = millis();
  }
 }
  if (millis() >= (lastTime2 + (90 * 1000)))
  {
   // getTemperature();
    lastTime2 = millis();
    }
  yield();

}