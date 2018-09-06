/*
   NEOPIXEL KITT - HTML UPDATER	
 ** remember to UPDATE SPIFFS THROUGH THE IDE WITH UPDATES TO THE SITEPAGES..!
     To upload through terminal you can use: curl -F "image=@firmware.bin" alfagt.local/update
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
/*
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ThingSpeak.h>
#include <FS.h>
#include <ArduinoJson.h>
#include "kittcmd.h"

#define DHTPIN 0 //// which is D6... or was it 14 (D5)?
#define DHTTYPE DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
unsigned long myChannelNumber = 511863;
const char *myWriteAPIKey = "ZPH42LNUJXJHUW0O";
const size_t bufferSize = JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(7) + 100;
DynamicJsonBuffer jsonBuffer(bufferSize);
*/

String conssoll = "";
uint8_t conssNum =0;
bool availCons=false;

float alfaTemp = 10;
float alfaHumid = 10;

#include "index.h"
MDNSResponder mdns;
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

boolean rolling, connd = false;
uint8_t rgb[3] = {0, 0, 200};
byte ranPixs = 0;
int stdDelaySec = 10;
int watsdoin = 99;
//const char ssid3[] = "TPG 15EA";
//const char pass3[] = "abcd1989";
const char ssid[] = "AlfaRomeoGT";
const char pass[] = "turismoGT";
const char ssid2[] = "AndroidAP";
const char pass2[] = "tttttttt";
const char *ssAPid = "node1T";
const char *passAP = "node";
const char *host = "node";

WiFiClient tsclient;
#define HOSTNAME "alfagt"
long lastTime1, lastTime2, lastTime3, lastTry = 0;

void stats()
{
   String json = "{";
   if(availCons){
    availCons=false;
   json+="\"c\":"+conssoll + "\",\"up\":\"";
  }else {
json += "\"up\":\"";
  }
  json += String(millis()) + "\"ran\":" + ranPixs + ",\"wd\":" + String(watsdoin) + "\",\"d\":\"" + String(stdDelaySec) + "\",\"r\":" + String(rolling) + ",\"rgb\":[\"" + String(rgb[0]) + "\",\"" + String(rgb[1]) + "\",\"" + String(rgb[2]) + "\"],\"t\":\"" + String(alfaTemp) + "\",\"h\":\"" + String(alfaHumid) + "\"}";
  server.send(200, "application/json", json);
}

void cmd()
{

}

void handRan() { server.send(200, "text/plain", String(ranPixs)); }
void handRoll(){   String roll = server.arg('rolling');  server.send(200, "text/plain", String(rolling));}
void handTemp() { server.send(200, "text/plain", String(alfaTemp)); }
void handHumid() { server.send(200, "text/plain", String(alfaHumid)); }
void handWd(){
  if (!server.hasArg("watsdoin"))server.send(200, "text/plain", String(watsdoin));
  String tt = server.arg("watsdoin");  watsdoin = tt.toInt();  server.send(200, "text/plain", String(watsdoin));}
void handDel(){
  if (!server.hasArg("delay"))    server.send(200, "text/plain", String(stdDelaySec));
  String LEDvalue = server.arg("delay");  stdDelaySec = LEDvalue.toInt();
  server.send(200, "text/plain", String(stdDelaySec));
}
void handleReset(){
  delay(1000);  ESP.restart();
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
  /*server.on("/cmd", cmd);
  server.on("/rgb", cmd);
  server.on("/delay", handDel);
  server.on("/watsdoin", handWd);
  server.on("/roll", handRoll);
  */
  server.on("/", handleRoot);
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
        Serial.println("WiFi failed, out.");
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

void handleRoot()
{
  String s = MAIN_page;
  server.send(200, "text/html", s);
}

void getTemperature()
{
/*  sensors_event_t event;
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
    return;
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
  ThingSpeak.setField(1, alfaTemp);
  ThingSpeak.setField(2, alfaHumid);
  ThingSpeak.setField(3, watsdoin);
  ThingSpeak.setField(5, ranPixs);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  Serial.println("written to thingSpeak");
  delay(200);
  */
}

void runThePix()
{
  long the = millis();
  ranPixs++;
  Serial.print("runnin");
  Serial.println(watsdoin);
  int doit = watsdoin;
  /*if (rolling)
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
  */
  Serial.println("done pix");
}

void setup(void)
{
  delay(1000);
  Serial.begin(115200);
  Serial.println();
 
 
 /*
 
 SPIFFS.begin();
  Serial.println("File System Initialized");
 setupOled();
  strip.begin();
  strip.show();
  colorWipe(blue(200), 5);
  Serial.println("STARTING");
  */
  stdDelaySec = 10;
  watsdoin = 99;
  rolling = false;
  connd = tryToConnect();
 /* if (connd)
  {
    theaterChase(green(200), 5);
  }
  else
  {
    theaterChase(red(200), 5);
  }

  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delay(100);
  ThingSpeak.begin(tsclient);
 */
 Serial.println("SETUP COMPLETE");
  delay(100);
}

void loop(void)
{
  server.handleClient();
  if (!connd)
  {
    if (millis() >= (lastTry + 60000))
    {
      Serial.println("no Wifi - searching for AP...");
      connd = tryToConnect();
      lastTry = millis();
    }
  }
  if (millis() >= (lastTime1 + (stdDelaySec * 1000)))
  {
    runThePix();
    lastTime1 = millis();
  }
  if (millis() >= (lastTime2 + (90 * 1000)))
  {
    getTemperature();
    lastTime2 = millis();
  }
  if (Serial.available()){
    availCons=true;
    conssoll = String(Serial.read());  
    }
  yield();
}