/**
* NEOPIXEL KITT BETA - PRODUCTION
* v8 - espserver
*  dht
* thinkspeak
* **/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
 
#include "index.h" 
//#include "graph.h" 
//#include "index2.h" 

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
#define PIN 4
#define NUMPIXELS 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 25;
String colour = "red";
String mode = "kitt";
int theSpeed = 25;
int cycles = 5;
int width = 4;
int stdDelaySec = 30;
int watsdoin = 1;
const char ssid[] = "Northern Frontier Interwebs";
const char pass[] = "num4jkha8nnk"; 
const char ssid3[] = "AlfaRomeoGT";
const char pass3[] = "turismoGT";
const char ssid2[] = "AndroidAP";
const char pass2[] = "tttttttt";
const char *ssAPid = "AlfaRomeo";
const char *passAP = "turismoGT";
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway2(192,168,43,1);
IPAddress ip2(192, 168, 43, 34);
IPAddress ipN(10, 0, 77, 34);
IPAddress gatewayN(10,0,77,100);
long theTime, lastTime1,lastTime2 = 0;
bool firstRun, secondRun,justRanPix=false;

#define DHTPIN     0    //// which is D6... or was it 14 (D5)?
#define DHTTYPE           DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
//unsigned long myChannelNumber = 404585;  key-W124WS7UN76VCASZ
unsigned long myChannelNumber = 511863;
const char * myWriteAPIKey = "ZPH42LNUJXJHUW0O";
char alfaTempC[6]; float alfaTemp=0;char alfaHumidC[6]; float alfaHumid=0;
int delField=4;int kittField = 3;int aTfield=1;int aHfield = 2;

void knightRider(uint16_t cyc, uint16_t spd, uint16_t wid, uint32_t color) {
  uint32_t old_val[NUMPIXELS]; // up to 256 lights!
  for (int i = 0; i < cyc; i++) {
    for (int count = 1; count<NUMPIXELS; count++) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for (int x = count; x>0; x--) {
        old_val[x - 1] = dimColor(old_val[x - 1], wid);
        strip.setPixelColor(x - 1, old_val[x - 1]);
      }
      strip.show();
      delay(spd);
    }
    for (int count = NUMPIXELS - 1; count >= 0; count--) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for (int x = count; x <= NUMPIXELS; x++) {
        old_val[x - 1] = dimColor(old_val[x - 1], width);
        strip.setPixelColor(x + 1, old_val[x + 1]);
      }
      strip.show();
      delay(spd);
    }
  }
  clearStrip();
}
void clearStrip() {
  for (int i = 0; i<NUMPIXELS; i++) {
    strip.setPixelColor(i, 0x000000); strip.show();
  }
}
uint32_t dimColor(uint32_t color, uint16_t wid) {
  return (((color & 0xFF0000) / wid) & 0xFF0000) + (((color & 0x00FF00) / wid) & 0x00FF00) + (((color & 0x0000FF) / wid) & 0x0000FF);
}
uint32_t colorWheel(byte WheelPos) {
  byte state = WheelPos / 21;
  switch (state) {
  case 0: return strip.Color(255, 0, 255 - ((((WheelPos % 21) + 1) * 6) + 127)); break;
  case 1: return strip.Color(255, ((WheelPos % 21) + 1) * 6, 0); break;
  case 2: return strip.Color(255, (((WheelPos % 21) + 1) * 6) + 127, 0); break;
  case 3: return strip.Color(255 - (((WheelPos % 21) + 1) * 6), 255, 0); break;
  case 4: return strip.Color(255 - (((WheelPos % 21) + 1) * 6) + 127, 255, 0); break;
  case 5: return strip.Color(0, 255, ((WheelPos % 21) + 1) * 6); break;
  case 6: return strip.Color(0, 255, (((WheelPos % 21) + 1) * 6) + 127); break;
  case 7: return strip.Color(0, 255 - (((WheelPos % 21) + 1) * 6), 255); break;
  case 8: return strip.Color(0, 255 - ((((WheelPos % 21) + 1) * 6) + 127), 255); break;
  case 9: return strip.Color(((WheelPos % 21) + 1) * 6, 0, 255); break;
  case 10: return strip.Color((((WheelPos % 21) + 1) * 6) + 127, 0, 255); break;
  case 11: return strip.Color(255, 0, 255 - (((WheelPos % 21) + 1) * 6)); break;
  default: return strip.Color(0, 0, 0); break;
  }
}
void kitt(){
  int t=30;
  knightRider(2, t, 4, 0xFF1000); // Cycles, Speed, Width, RGB Color (original orange-red)
  t=t-5;
  knightRider(2, t, 3, 0xFF00FF); // Cycles, Speed, Width, RGB Color (purple)
  t=t-5;
  knightRider(2, t, 2, 0x0000FF); // Cycles, Speed, Width, RGB Color (blue)
  t=t-5;
  knightRider(2, t, 5, 0xFF0000); // Cycles, Speed, Width, RGB Color (red)
  t=t-5;
  knightRider(2, t, 4, 0x00FF00); // Cycles, Speed, Width, RGB Color (green)
  t=t+5;
  knightRider(2, t, 4, 0xFFFF00); // Cycles, Speed, Width, RGB Color (yellow)
  t=t+5;
  knightRider(2, t, 4, 0x00FFFF); // Cycles, Speed, Width, RGB Color (cyan)
  t=t+5;
  knightRider(2, t, 2, 0xFFFFFF); // Cycles, Speed, Width, RGB Color (white)
  clearStrip();
}
void copMode(){
  int del = 20;
  for (int cy=1;cy<=5;cy++){
  // set to 5 cycles...
  knightRider(1, del, 5, 0xFF0000); // red
  knightRider(1, del, 5, 0x0000FF); // blue         
  del=(del-2);
  }
}
void runThePix() {
  Serial.println("runnin pix");
  if (watsdoin==0) {
   Serial.println("off");
  } 
  else if (watsdoin==1) {
    knightRider(1, 18, 3, 0xFF1000); // (original orange-red)
    knightRider(1, 20, 3, 0xFF1000); // (original orange-red)
    knightRider(1, 22, 3, 0xFF1000); // (original orange-red)
    knightRider(1, 25, 4, 0xFF1000); // (original orange-red)
    knightRider(1, 30, 3, 0xFF1000); // (original orange-red)
    knightRider(1, 35, 3, 0xFF1000); // (original orange-red)
  }
  else if (watsdoin==7){
    kitt();     
    } 
  else if (watsdoin==4){
    copMode();
    } 
  else if (watsdoin==2){
    knightRider(5, 25, 3, 0xFFFF00);        // yellow
  }
  else if (watsdoin==5){
      knightRider(5, 25, 3, 0x00FF00);      // green
  } 
  else if (watsdoin==8){
    knightRider(5, 25, 3, 0x0000FF);      // blue         
  }
}
void handleRoot() {
  if (server.hasArg("mode")) {
    handMode();
  }
  if (server.hasArg("watsdoin")) {
   Serial.print('hwd');
   handWd();
  }
    if (server.hasArg("delay")) {
    Serial.print('hdel');
  handDelay();
  }
  else {
    String s = MAIN_page;
    server.send(200, "text/html", s);
  }
}

void returnFail(String msg)
{
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(500, "text/plain", msg + "\r\n");
}

void handMode()
{
  Serial.print('hmofe');
  if (!server.hasArg("mode")) server.send(200, "text/plain", mode);
  String LEDvalue = server.arg("mode");
  mode = LEDvalue;
  if (LEDvalue == "kitt") {
    watsdoin=1;
    mode="kittStd";
  }
  if (LEDvalue == "multi") {
    watsdoin=7;
    mode="multi";
  }
  if (LEDvalue == "cop") {
    watsdoin=4;
    mode="cop";
  }
  if (LEDvalue == "blue") {
    watsdoin=8;
    }
  if (LEDvalue == "yellow") {
    watsdoin=2;
      }
  if (LEDvalue == "green") {
    watsdoin=5;
  }
   server.send(200, "text/plain", mode);
}

void handTemp() {
 server.send(200, "text/plain", String(alfaTemp));
}
void handHumid() {
 server.send(200, "text/plain", String(alfaHumid)); 
}
void handWd() {
if (!server.hasArg("watsdoin")) server.send(200, "text/plain", String(watsdoin)); 
String tt= server.arg("watsdoin");
   watsdoin =tt.toInt();
  server.send(200, "text/plain", String(watsdoin));
}
void handDelay(){
    String LEDvalue;
    if (!server.hasArg("delay")) server.send(200, "text/plain", String(stdDelaySec));
    LEDvalue = server.arg("delay");
    stdDelaySec=LEDvalue.toInt();
    server.send(200, "text/plain", String(stdDelaySec));
}

void getTemperature() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    alfaTemp = event.temperature;
Serial.println(" *C");
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
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
  ThingSpeak.setField(delField,String(stdDelaySec));
  ThingSpeak.setField(8,String(millis()));
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  Serial.println("written to thingSpeak");
  delay(200);
}

void returnOK(){
  server.sendHeader("Connection", "close");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "text/plain", "OK\r\n");
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  Serial.println("STARTING");
  stdDelaySec = 30;
  firstRun = true;
  watsdoin = 1;
 String hostname(HOST_NAME);
  WiFi.hostname(hostname);
 int n = WiFi.scanNetworks();
 for (int i = 0; i < n; ++i){
 String tryWi = WiFi.SSID(i);
 if (tryWi == ssid2) {
 Serial.println("CONNECTED TO ANDROIDAP");// WiFi.config(ip2,gateway2,subnet);
 WiFi.begin(ssid2, pass2);
 } else if (tryWi == ssid) {
//  WiFi.config(ip,gateway,subnet);
 WiFi.begin(ssid, pass);
 Serial.println("CONNECTED TO NTHRN INTERWEBS");
  }
   else if (tryWi == ssid3) {
 Serial.println("CONNECTED TO ALFAWIFI"); //WiFi.config(ip2,gateway2,subnet);
 WiFi.begin(ssid3, pass3);
  }
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
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  delay(50);
  server.on("/", handleRoot);
  server.on("/mode", handMode);
  server.on("/delay", handDelay);
  server.on("/watsdoin", handWd); server.on("/temp", handTemp); server.on("/humid", handHumid);
  server.onNotFound(handleNotFound);
  server.begin();
  delay(50);
    dht.begin();
    sensor_t sensor; dht.temperature().getSensor(&sensor); Serial.println("------------------------------------"); Serial.println("Temperature");  Serial.print  ("Sensor:       "); Serial.println(sensor.name);  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version); Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C"); Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C"); Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C"); Serial.println("------------------------------------");
    dht.humidity().getSensor(&sensor); Serial.println("------------------------------------");  Serial.println("Humidity");  Serial.print  ("Sensor:       "); Serial.println(sensor.name);  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id); Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  Serial.println("------------------------------------");
    delay(500);
    ThingSpeak.begin(client);
if (!MDNS.begin("alfagt")) {
   Serial.println("Error setting up MDNS responder!");
  }
  MDNS.addService("http", "tcp", 80);
   delay(40);
  strip.begin();
  strip.show();
  Serial.print("Connected");
  Serial.println(String(WiFi.localIP()));
  delay(40);
}
}

void loop(void) {
 ArduinoOTA.handle();
 theTime = millis();
server.handleClient();
if (firstRun) {
firstRun = false;
kitt();
}
if (justRanPix){
    justRanPix=false;
    Serial.print("wats=");
    Serial.println(watsdoin);
    Serial.println("mode =");
    Serial.println(mode);
   }
 if (theTime >= (lastTime1 + (stdDelaySec * 1000))) {
    lastTime1=theTime;
    runThePix();
    strip.clear();
    justRanPix=true;
  }
if (theTime >= (lastTime2 + (90 * 1000))) {
    lastTime2=theTime;
  getTemperature();
    }
 yield();
}
