/**
* NEOPIXEL KITT BETA - PRODUCTION
* v8abb - perfectionistBRANCH
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
#include <ESP8266Ping.h>

const IPAddress remote_ip(74,125,224,72);
bool noInt=true;
MDNSResponder mdns;
ESP8266WebServer server(80);
WiFiClient client;
#define HOST_NAME "alfagtttt"
#include "kittcmd.h"


byte r,g,b=0;
int stdDelaySec = 10;
int watsdoin = 1;
const char ssid3[] = "AlfaRomeoGT";
const char pass3[] = "turismoGT";
const char ssid2[] = "AndroidAP";
const char pass2[] = "tttttttt";
const char ssid[] = "TPG 15EA";
const char pass[] = "abcd1989";
const char *ssAPid = "AlfaRomeo";
const char *passAP = "turismoGT";
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway2(192,168,43,1);
IPAddress ip2(192, 168, 43, 34);
long theTime, lastTime1,lastTime2,lastTime3,lastTry,nextRun = 0;
bool firstRun, secondRun,justRanPix,connd,rolling=false;

#define DHTPIN     12    //// which is D6... or was it 14 (D5)?
#define DHTTYPE           DHT11
DHT_Unified dht(DHTPIN, DHTTYPE);
//unsigned long myChannelNumber = 404585;  key-W124WS7UN76VCASZ
unsigned long myChannelNumber = 511863;
const char * myWriteAPIKey = "ZPH42LNUJXJHUW0O";
char alfaTempC[6]; float alfaTemp=10;char alfaHumidC[6]; float alfaHumid=10;
int delField=4;int kittField = 3;int aTfield=1;int aHfield = 2;


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
  server.send(200, "text/plain", tt);
}
void handDelay(){
  if (!server.hasArg("delay")) server.send(200, "text/plain", String(stdDelaySec)); 
    String LEDvalue = server.arg("delay");
    stdDelaySec=LEDvalue.toInt();
    server.send(200, "text/plain", String(stdDelaySec));
}
void handlergb(){
String rr="";
if (server.hasArg("r")) {
rr =server.arg("r");
r = rr.toInt(); 
}
if (server.hasArg("b")) {
rr =server.arg("b");
b=rr.toInt();
}
if (server.hasArg("g")) {
rr=server.arg("g");
g=rr.toInt();
}
String tt= "r="+String(r)+"&g="+String(g)+"&b="+String(b);
server.send(200, "text/plain", tt);
}


void handRoll(){
if (!server.hasArg("rolling")) server.send(200, "text/plain", String(rolling)); 
    String value = server.arg("rolling");
    rolling=value.toInt();
    server.send(200, "text/plain", String(rolling));
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
  if (server.hasArg("r") || server.hasArg("g") || server.hasArg("b")) {
    handlergb();
  }
    if (server.hasArg("rolling")) {
    handRoll();
  }
  if (server.hasArg("watsdoin")) {
    handWd();
  }
    if (server.hasArg("delay")) {
   handDelay();
  }
  else {
  if (noInt){
    String s = NOINT_page;
    server.send(200, "text/html", s);
  } else {
   String s = MAIN_page;
    server.send(200, "text/html", s);
  }
}
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
bool tryToConnect(const char ssidd[],const char passd[])
{
   Serial.println("Connecting Wifi...");
   WiFi.begin(ssidd, passd);
    for (int t=0;t<10;t++){
  while (WiFi.status()  != WL_CONNECTED) {  
  	delay(20);
	} 
     Serial.print("CONN TRY SUCCESS - ");Serial.println(ssidd);
bool gotInt= Ping.ping(remote_ip);
delay(1500);
if (gotInt){
Serial.println("SUCCESS in pinging google.com");
noInt=true;
  } else {
    Serial.println("Error :(");
	Serial.println("Cant connect to google");
	noInt=false;
  }
	return true;
  }
  Serial.print("CONN TRY FAILED - ");
 Serial.println(ssidd);
 WiFi.softAP(ssAPid, passAP);
 delay(1000);
return false; 
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

void runThePix() {
  Serial.println("runnin pix");
  if (watsdoin==0) {
   Serial.println("off");
  }   
  else if (watsdoin==1) {
    knightRider(5, 35, 3, 0xFF1000); // (original orange-red)
  }
  else if (watsdoin==2){
  uint32_t col = rgb2hex(r,g,b);
  knightRider(5, 25, 3, col); 
  }
  else if (watsdoin==3){
  oldLoop();
  }
    else if (watsdoin==4){
    copMode();
    } 
	
	
	else if (watsdoin==6){
 newLoop();
  }	
    else if (watsdoin==7){
    rainbow();     
    } 
	
	
	else if (watsdoin==9){
   chaseMe(150);
  }	
  else if (watsdoin == 10)
  {
    theaterChase(255, 10);
  }
  else if (watsdoin == 11)
  {
    theaterChaseRainbow(150);
  }
   else if (watsdoin == 12)
  {
pulseWhite(450);  
  }
  else if (watsdoin == 13)
 {
rainbowFade2White(10,5,2);
 }
   else if (watsdoin == 14)
  {
   copMode2();
  }
   else if (watsdoin == 15)
  {
  whiteOverRainbow(25,75,8);
  }
  if(rolling){
  watsdoin++;
if (watsdoin==16) {
watsdoin=1;
Serial.println("back to WD 1");
}
}
 Serial.println("done pix");
 }


void setup(void)
{
pinMode(BUILTIN_LED, OUTPUT);
  strip.begin();
  strip.show();
  colorWipe(150,5);   
  Serial.begin(115200);
  Serial.println("STARTING");
  stdDelaySec = 10;
  firstRun = true;
  watsdoin = 1;
  rolling=false;
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
server.on("/r", handlergb);
server.on("/b", handlergb);
server.on("/g", handlergb);
server.on("/", handleRoot);
  server.on("/rolling", handRoll);
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
    MDNS.addService("http", "tcp", 80);
}

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


void loop(void) {
theTime = millis();
ArduinoOTA.handle();
server.handleClient();
if (firstRun) {
firstRun = false;
kitt();
}
if (WiFi.status() != WL_CONNECTED){
   if (theTime >= (lastTry + 60000)) {
   lastTry=theTime;
  connd = tryToConnect(ssid3, pass3);
  delay(1750);
   if(!connd)
  {
 connd=tryToConnect(ssid2, pass2); 
   delay(1750);
   if(!connd)
  {
  connd=tryToConnect(ssid, pass); 
  delay(1750);
 }
 }
 }
 }

 if (theTime >= (lastTime1 + (stdDelaySec * 1000))) {
    runThePix();
    lastTime1=theTime;
    strip.clear();
    justRanPix=true;
  }
if (theTime >= (lastTime2 + (90 * 1000))) {
    lastTime2=theTime;
  getTemperature();
    }
 yield();
}
