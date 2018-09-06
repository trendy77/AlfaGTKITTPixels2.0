 /**
* NEOPIXEL KITT BETA - PRODUCTION
* v6.1 - aREST implementation
*  JSON added
* **/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#include <ESP8266WiFi.h>
#include <aREST.h>
#include <ArduinoJson.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

aREST rest = aREST();
#define LISTEN_PORT           80
WiFiServer restserver(LISTEN_PORT);
#define PIN 4
#define NUMPIXELS 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 500;
MDNSResponder mdns;

String modeS = "kitt";
int speed = 25;
int cycles = 5;
int width = 4;
int stdDelaySec = 30;
int red,green,blue = 200;
int watsdoin = 1;

IPAddress subnet(255, 255, 255, 0);
IPAddress ip(10, 0, 77, 151);
IPAddress gateway(10, 0, 77, 100);
const char ssid[] = "Northern Frontier Interwebs";
const char pass[] = "num4jkha8nnk";
//const char ssid3[] = "AlfaRomeoGT";
//const char pass3[] = "turismoGT";
bool firstRun, secondRun, off = false;
//const char ssid2[] = "AndroidAP";
//const char pass2[] = "tttttttt";
long theTime, lastTime1 = 0;
const size_t bufferSize = JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(7) + 90;
DynamicJsonBuffer jsonBuffer(bufferSize);

const char* json = "{\"off\":false,\"mode\":\"kitt\",\"delaySecs\":30,\"speed\":25,\"width\":3,\"cycles\":5,\"colour\":[200,200,200]}";
String jsonS="";

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
      delay(speed);
    }
  }
  clearStrip();
}
void clearStrip(){
  for (int i = 0; i<NUMPIXELS; i++) {
    strip.setPixelColor(i, 0x000000); strip.show();
  }
  }
uint32_t dimColor(uint32_t color, uint16_t wid) {
  return (((color & 0xFF0000) / wid) & 0xFF0000) + (((color & 0x00FF00) / wid) & 0x00FF00) + (((color & 0x0000FF) / wid) & 0x0000FF);
}
uint32_t colorWheel(byte WheelPos){
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
  knightRider(3, 12, 4, 0xFF1000); // Cycles, Speed, Width, RGB Color (original orange-red)
  knightRider(3, 17, 3, 0xFF00FF); // Cycles, Speed, Width, RGB Color (purple)
  knightRider(3, 22, 2, 0x0000FF); // Cycles, Speed, Width, RGB Color (blue)
  knightRider(3, 27, 5, 0xFF0000); // Cycles, Speed, Width, RGB Color (red)
  knightRider(3, 32, 6, 0x00FF00); // Cycles, Speed, Width, RGB Color (green)
  knightRider(3, 32, 7, 0xFFFF00); // Cycles, Speed, Width, RGB Color (yellow)
  knightRider(3, 32, 8, 0x00FFFF); // Cycles, Speed, Width, RGB Color (cyan)
  knightRider(3, 32, 2, 0xFFFFFF); // Cycles, Speed, Width, RGB Color (white)
  clearStrip();
  delay(2000);
}
void rainbow(){
  for (byte j = 0; j<252; j += 7) {
    knightRider(1, 16, 2, colorWheel(j)); // Cycles, Speed, Width, RGB Color
  }
  clearStrip();
}
void parse(const char * json){
  JsonObject& root = jsonBuffer.parseObject(json);
  // Test if parsing succeeds.
    if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
    } else {
  // Fetch values.
off = root["off"]; // false
const char* mode = root["mode"]; // "kitt"
modeS=String(mode);
stdDelaySec = root["delaySecs"]; // 30
speed = root["speed"]; // 25
width = root["width"]; // 3
cycles = root["cycles"]; // 5
JsonArray& colour = root["colour"];
red= colour[0]; // 200
green = colour[1]; // 200
blue = colour[2]; // 200 
}
}
void copMode(){
      for (int i = 0; i < NUMPIXELS; i + 2) {
        strip.setPixelColor(i, strip.Color(170, 0, 0)); // RED
        strip.setPixelColor(i + 1, strip.Color(0, 0, 170)); // BLUE EACH 2ND 
        strip.show(); // This sends the updated pixel color to the hardware.
        delay(delayval); // Delay for a period of time (in milliseconds).
      }
      for (int i = 0; i < NUMPIXELS; i + 2) {
        strip.setPixelColor(i, strip.Color(0, 0, 170)); // RED
        strip.setPixelColor(i + 1, strip.Color(170, 0, 0)); // BLUE EACH 2ND 
        strip.show(); // This sends the updated pixel color to the hardware.
        delay(delayval); // Delay for a period of time (in milliseconds).
      }
  delay(1000);
 clearStrip();
 }
void runThePix(){
  Serial.print("IP runThePix: ");
	if (modeS = "kitt"){
		  knightRider(cycles, speed, width, 0xFF1000); // (original orange-red)
	} else if (modeS = "rainbow"){
	   rainbow();  
	}else if (modeS = "cop"){
	  copMode();
	} else if (modeS = "solid"){
		knightRider(cycles, speed, width, 0x00FF00);      // green
		knightRider(cycles, speed, width, 0xFFFF00);        // yellow
		knightRider(cycles, speed, width, 0x0000FF);      // blue         
	}
}
int kittControl(String Command){
	const char * theJson = Command.c_str();
	parse(theJson);
	return 1;
}


void setup(void) {
  Serial.begin(115200);
  stdDelaySec = 10;
  firstRun = true;
  watsdoin = 1;
  /*rest.variable("watsdoin", &watsdoin);
  rest.variable("speed", &speed);
  rest.variable("width",&width);
  rest.variable("red",&red);
  rest.variable("blue",&blue);
  rest.variable("green",&green);
  rest.variable("delaySecs",&stdDelaySec);
  rest.function("kittControl", kittControl);
  rest.set_id("1");
  rest.set_name("turismoGT");
 */
  Wifi.config(ip,gateway,subnet);
  Wifi.begin(ssid,pass);
 // int n = WiFi.scanNetworks();
  //Serial.println("scan done");
  while (WiFi.status() != WL_CONNECTED)    {
    delay(500);
    Serial.print(".");
  }
	//for (int i = 0; i < n; ++i) {
   // String tryWi = WiFi.SSID(i);
   // if (tryWi == ssid) {
   // }
   // else if (tryWi == ssid2) {
    //  WiFi.begin(ssid2, pass2);
   // }
    //else if (tryWi == ssid3) {
 //     WiFi.begin(ssid3, pass3);
   // }
 // }
  
	  Serial.print("IP address: ");
	  Serial.println(WiFi.localIP());
	  ArduinoOTA.setHostname("AlfaGT");
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
  restserver.begin();
  strip.begin();
  strip.show();
  Serial.println("ALFAPixels server started");
  delay(500);
  kitt();
}

void loop(void) {
  theTime = millis();
  ArduinoOTA.handle();
  if (firstRun) {
    rainbow();
    firstRun = false;
    secondRun = true;
    } if (secondRun) {
    copMode();
    secondRun = false;
  }
  WiFiServer client = restserver.available();
  if (WiFi.status() == WL_CONNECTED) {
    jsonS ="";
    int numChars=0;
    if (client.connected){
      while (client.available()){
      char c = client.read();
      json += c;
      }
      /// SENDING
     const char * mode = modeS.c_str();
    JsonObject& root = jsonBuffer.createObject();
    root["off"] = off;
    root["mode"] = mode;
   modeS=String(mode); 
   root["delaySecs"] = stdDelaySec;
    root["speed"] = speed;
    root["width"] = width;
    root["cycles"] = cycles;
    JsonArray& colour = root.createNestedArray("colour");
    colour.add(red);
    colour.add(green);
    colour.add(blue);
    root.printTo(client);
  // Write response headers
  client.println("HTTP/1.0 200 OK");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.println();
  // Write JSON document
  root.prettyPrintTo(client);
  // Disconnect
  client.stop();
   kittControl(json);
    }
  }
  rest.handle(client);
  if (theTime >= (lastTime1 + (stdDelaySec * 1000))) {
    runThePix();
    strip.clear();
    }
  }

