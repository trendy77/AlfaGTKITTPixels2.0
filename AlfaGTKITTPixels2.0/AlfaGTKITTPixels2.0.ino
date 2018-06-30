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

MDNSResponder mdns;
ESP8266WebServer server(80);
WiFiClient client;
#define HOST_NAME "alfagt"
#include "kittcmd.h"

//String colour = "red";String mode = "kitt";int theSpeed = 25;int cycles = 5;int width = 4;
int stdDelaySec = 30;
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
long theTime, lastTime1, lastTime2, lastTime3, lastTry, nextRun = 0;
bool connd, firstRun, secondRun, boutRunPix, wifConnected = false;

#define DHTPIN 12 //// which is D6... or was it 14 (D5)?
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
//unsigned long myChannelNumber = 404585;  key-W124WS7UN76VCASZ
unsigned long myChannelNumber = 511863;
const char *myWriteAPIKey = "ZPH42LNUJXJHUW0O";
char alfaTempC[6];
float alfaTemp = 10;
char alfaHumidC[6];
float alfaHumid = 10;
int delField = 4;
int kittField = 3;
int aTfield = 1;
int aHfield = 2;
int numRanfield = 5;

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
boolean tryToConnect(const char ssidd[],const char passd[])
{
  WiFi.begin(ssidd, passd);
  Serial.println("Connecting Wifi...");
  for (int t=0;t<10;t++){
  if (WiFi.waitForConnectResult() == WL_CONNECTED) {  
      digitalWrite(BUILTIN_LED, LOW); 
      return true;
    } 
     delay(10);
     Serial.println(".");
    t++;
  } 
return false; 
}
bool runningNOW=false;

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
  delay(50);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssAPid,passAP);
  //ESP.restart();
  }
  digitalWrite(BUILTIN_LED, LOW);
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
  theTime = millis();
  ArduinoOTA.handle();
  server.handleClient();
  if (firstRun)
  {
    firstRun = false;
    newLoop();
  }
  if (!connd){
  if(theTime >= (lastTry + 60000)){
  WiFi.mode(WIFI_STA);
  lastTry = theTime;
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

  if (theTime >= (lastTime3 + 2000))
  {
    long lastRun = (theTime-lastTime1);
    Serial.print("last1 was ");
    Serial.println(lastRun);
      nextRun = ((stdDelaySec * 1000)-lastRun);
      Serial.print("nextrun=");
   Serial.println(nextRun);
   }

  if (theTime >= (lastTime1 + (stdDelaySec * 1000)))
  {
    if (!runningNOW){
    runThePix();
    runningNOW=false;
    lastTime1 = theTime;
} else {
    Serial.println("one pixelrun skipped");
    lastTime1 = theTime;
  }
 }
  if (theTime >= (lastTime2 + (90 * 1000)))
  {
    getTemperature();
    lastTime2 = theTime;
    }
  yield();

}