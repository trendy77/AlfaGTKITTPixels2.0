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
// experimental extra kitt functions...
#include "kittset.h"

//String colour = "red";String mode = "kitt";int theSpeed = 25;int cycles = 5;int width = 4;
int stdDelaySec = 30;
int watsdoin = 1;
//const char ssid[] = "Northern Frontier Interwebs";
//const char pass[] = "num4jkha8nnk"; 
//IPAddress ipN(10, 0, 77, 34);
//IPAddress gatewayN(10,0,77,100);
//const char ssid3[] = "AlfaRomeoGT";
//const char pass3[] = "turismoGT";

const char ssid2[] = "AndroidAP";
const char pass2[] = "tttttttt";
//AP-STA
const char *ssAPid = "AlfaRomeo";
const char *passAP = "turismoGT";
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway2(192,168,43,1);
IPAddress ip2(192, 168, 43, 34);
long theTime, lastTime1,lastTime2,lastTime3,lastTry,nextRun = 0;
bool firstRun, secondRun,justRanPix,wifConnected=false;

#define DHTPIN     0    //// which is D6... or was it 14 (D5)?
#define DHTTYPE           DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
//unsigned long myChannelNumber = 404585;  key-W124WS7UN76VCASZ
unsigned long myChannelNumber = 511863;
const char * myWriteAPIKey = "ZPH42LNUJXJHUW0O";
char alfaTempC[6]; float alfaTemp=10;char alfaHumidC[6]; float alfaHumid=10;
int delField=4;int kittField = 3;int aTfield=1;int aHfield = 2;

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
  if (server.hasArg("nextrun")) {
    handNext();
  }
    if (server.hasArg("temp")) {
    handTemp();
  }
    if (server.hasArg("humid")) {
    handHumid();
  }
  if (server.hasArg("watsdoin")) {
    handWd();
  }
    if (server.hasArg("delay")) {
   handDelay();
  }
  else {
    String s = MAIN_page;
    server.send(200, "text/html", s);
  }
}
void handNext(){
server.send(200, "text/plain", String(nextRun));
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
    LEDvalue = server.arg("delay");
    stdDelaySec=LEDvalue.toInt();
    server.send(200, "text/plain", String(stdDelaySec));
}
void handleReset(){
  delay(1000);
    ESP.restart();
}
void tryToConnect(){
int n = WiFi.scanNetworks();
for (int i = 0; i < n; ++i){
String tryWi = WiFi.SSID(i);
if (tryWi == ssid2) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid2, pass2);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
delay(500);
}
}
}
}
/**
 } else if (tryWi == ssid) {
 WiFi.config(ip,gateway,subnet);
 WiFi.begin(ssid, pass);
 Serial.println("CONNECTED TO NTHRN INTERWEBS");
  }
   else if (tryWi == ssid3) {
 Serial.println("CONNECTED TO ALFAWIFI"); //WiFi.config(ip2,gateway2,subnet);
 WiFi.begin(ssid3, pass3);
**/
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
 String htname="alfagt.local";
 String hostname(htname);
  WiFi.hostname(hostname);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid2, pass2);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed!");
    delay(5000);
    //ESP.restart();
  }
 delay(500);
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
  server.on("/reset", handleReset);
  server.on("/nextrun", handNext);
  server.on("/delay", handDelay);
  server.on("/watsdoin", handWd); 
  server.on("/temp", handTemp);
  server.on("/humid", handHumid);
  server.onNotFound(handleNotFound);
  server.begin();
  delay(50);
if (!MDNS.begin("alfagt")) {
   Serial.println("Error setting up MDNS responder!");
  }
  MDNS.addService("http", "tcp", 80);
  delay(40);
  strip.begin();
  strip.show();
    delay(200);  
    dht.begin();
    sensor_t sensor; dht.temperature().getSensor(&sensor); Serial.println("------------------------------------"); Serial.println("Temperature");  Serial.print  ("Sensor:       "); Serial.println(sensor.name);  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version); Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C"); Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C"); Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C"); Serial.println("------------------------------------");
    dht.humidity().getSensor(&sensor); Serial.println("------------------------------------");  Serial.println("Humidity");  Serial.print  ("Sensor:       "); Serial.println(sensor.name);  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id); Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  Serial.println("------------------------------------");
    delay(200);
    ThingSpeak.begin(client);
  Serial.println(String(WiFi.localIP()));
  delay(40);
}

void loop(void) {
theTime = millis();
ArduinoOTA.handle();
server.handleClient();
if (firstRun) {
firstRun = false;
kitt();
}
if (WiFi.status() != WL_CONNECTED){
   Serial.println("ntConn?");
   if (theTime >= (lastTry + 60000)) {
    lastTry=theTime;
    tryToConnect();
    }
 }
if (theTime >= (lastTime3 + 5000)) {       ///justRanPix){
lastTime3=theTime;
nextRun = (theTime -(lastTime1 + (stdDelaySec * 1000)));  
Serial.print("wats=");Serial.println(watsdoin);
    Serial.print("del="); Serial.println(stdDelaySec);
    Serial.print("nextrun in");  Serial.println(nextRun);
///    justRanPix=false;
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
