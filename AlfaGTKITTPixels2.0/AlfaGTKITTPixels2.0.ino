/**
* NEOPIXEL KITT BETA - PRODUCTION
* v6.1 - aREST implementation
*  REST
* **/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='radio' name='mode' value='kitt' checked><input type='submit' value='Update'></form>";
#include <WiFiClient.h>

#define LISTEN_PORT           80
ESP8266WebServer server(LISTEN_PORT);
#define HOST_NAME "alfagt"
#define PIN 4
#define NUMPIXELS 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 50;

String colour = "red";
String modeS = "kitt";
int theSpeed = 25;
int cycles = 5;
int width = 4;
int stdDelaySec = 30;
//int red,blue,green=200;
int watsdoin = 1;
const char ssid[] = "Northern Frontier Interwebs";
const char pass[] = "num4jkha8nnk"; 
const char ssid3[] = "AlfaRomeoGT";
const char pass3[] = "turismoGT";
const char ssid2[] = "AndroidAP";
const char pass2[] = "tttttttt";
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway2(192,168,43,1);
IPAddress ip2(10, 0, 77, 34);
IPAddress gateway(192,168,43,1);
IPAddress ip(192, 168, 43, 34);
long theTime, lastTime1 = 0;
bool firstRun, secondRun, off = false;


void setLights(int cmdr){
watsdoin=cmdr;
}

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
      delay(delayval);
    }
    for (int count = NUMPIXELS - 1; count >= 0; count--) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for (int x = count; x <= NUMPIXELS; x++) {
        old_val[x - 1] = dimColor(old_val[x - 1], width);
        strip.setPixelColor(x + 1, old_val[x + 1]);
      }
      strip.show();
      delay(delayval);
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
void kitt() {
  knightRider(3, 12, 4, 0xFF1000); // Cycles, Speed, Width, RGB Color (original orange-red)
  knightRider(3, 17, 3, 0xFF00FF); // Cycles, Speed, Width, RGB Color (purple)
  knightRider(3, 22, 2, 0x0000FF); // Cycles, Speed, Width, RGB Color (blue)
  knightRider(3, 27, 5, 0xFF0000); // Cycles, Speed, Width, RGB Color (red)
  knightRider(3, 12, 6, 0x00FF00); // Cycles, Speed, Width, RGB Color (green)
  knightRider(3, 12, 7, 0xFFFF00); // Cycles, Speed, Width, RGB Color (yellow)
  knightRider(3, 12, 8, 0x00FFFF); // Cycles, Speed, Width, RGB Color (cyan)
  knightRider(3, 12, 2, 0xFFFFFF); // Cycles, Speed, Width, RGB Color (white)
  clearStrip();
}
void copMode(){
  for (int cy=1;cy<=5;cy++){
  // set to 5 cycles...
  for (int wide=3;wide<=16;wide++){
  knightRider(1, 25, wide, 0xFF0000); // red
  knightRider(1, 25, wide, 0x0000FF); // blue         
  delay(200);
  }
  }
}
void runThePix() {
  Serial.println("runnin pix");
  if (watsdoin==1) {
    knightRider(cycles, theSpeed, width, 0xFF1000); // (original orange-red)
  } 
  else if (watsdoin==7){
    //rainbow();
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
String upCalc(unsigned long elapsed){
float h,m,s,ms;
 unsigned long over;
  h=int(elapsed/3600000);
  over=elapsed%3600000;
  m=int(over/60000);
  over=over%60000;
  s=int(over/1000);
  ms=over%1000;
 String ret = String(h)+":"+String(m)+","+String(s)+"."+String(ms);
 return ret;
}
void handleRoot() {
  String zeetime = upCalc(theTime);
  String html = "<!DOCTYPE html><html><head><title>AlfaRomeoGT</title></head><div class=\"w3-col l4 m12\" style=\"text-align:center;\"><h1>AlfaRomeoGT Neopixel</h1><p>Uptime: ";
 html += zeetime;
 html += "</p><br><h4>KITT IS CURRENTLY ";
 if (off){  html += "OFF";
 }else{
  html += "ON";
 }
 html += "<br><p>watsdoin: ";
 html += String(watsdoin);
 html += "</p><br><p>StdDelay: ";
 html += String(stdDelaySec);
 html += " secs</p><br><table><tr><th>SPECIAL MODES</th><th>SOLID COLOURS</th><th>DELAY TIME</th><tr><td><a href=\"0\">OFF</a></td></tr><tr><td><a href=\"1\">1 STD MODE</a></td><td><a href=\"2\">2 - GREEN</a></td><td><a href=\"3\">3 - 10 sec delay</a></td></tr><tr><td><a href=\"4\">4 - COP MODE</a></td><td><a href=\"5\">5 - YELLOW</a></td><td><a href=\"6\">6 - 30 sec delay</a></td></tr><tr><td><a href=\"7\">7 - RAINBOW</a></td><td><a href=\"8\">8 - BLUE</a></td><td><a href=\"9\">9 - 60 sec delay</a></td></tr></table></body></html>";
 server.send(200, "text/html", html);
 delay(200);
}

void handleRootCmd(int cmd) {
  Serial.println("hndl root cmd");
  watsdoin=cmd;
  String zeetime = upCalc(theTime);
 String html = "<!DOCTYPE html><html><head><title>AlfaRomeoGT</title></head><div class=\"w3-col l4 m12\" style=\"text-align:center;\"><h1>AlfaRomeoGT Neopixel</h1><p>Uptime: ";
  html += zeetime;
 html += "</p><br><h4>KITT IS CURRENTLY ";
 if (off){  html += "OFF";
 }else{
  html += "ON";
 }
 html += "<br><p>watsdoin: ";
 html += String(watsdoin);
 html += "</p><br><p>StdDelay: ";
 html += String(stdDelaySec);
 html += "</p><br><table><tr><th>SPECIAL MODES</th><th>SOLID COLOURS</th><th>DELAY TIME</th><tr><td><a href=\"0\">OFF</a></td></tr><tr><td><a href=\"1\">1 STD MODE</a></td><td><a href=\"2\">2 - GREEN</a></td><td><a href=\"3\">3 - 10 sec delay</a></td></tr><tr><td><a href=\"4\">4 - COP MODE</a></td><td><a href=\"5\">5 - YELLOW</a></td><td><a href=\"6\">6 - 30 sec delay</a></td></tr><tr><td><a href=\"7\">7 - RAINBOW</a></td><td><a href=\"8\">8 - BLUE</a></td><td><a href=\"9\">9 - 60 sec delay</a></td></tr></table></body></html>";
  server.send(200, "text/html", html);
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
  stdDelaySec = 30;
  firstRun = true;
  watsdoin = 1;
  String hostname(HOST_NAME);
  WiFi.hostname(hostname);
 int n = WiFi.scanNetworks();
 for (int i = 0; i < n; ++i) {
   String tryWi = WiFi.SSID(i);
   if (tryWi == ssid) {
  WiFi.config(ip,gateway,subnet);
  WiFi.begin(ssid, pass);
   } else if (tryWi == ssid2) {
  WiFi.config(ip2,gateway2,subnet);
  WiFi.begin(ssid2, pass2);
  } else if (tryWi == ssid3) {
 WiFi.config(ip2,gateway2,subnet);
 WiFi.begin(ssid3, pass3);
   }
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
  delay(200);
  server.on("/", handleRoot);
  server.on("/0", [](){
  setLights(0);
  
  });
  server.on("/1", [](){
  watsdoin=1;
  handleRoot();
  });
  server.on("/2", [](){
  handleRootCmd(2);
  });
  server.on("/3", [](){
  handleRootCmd(3);
  });
  server.on("/4", [](){
  watsdoin=4;
  handleRootCmd(4);
  });
  server.on("/5", [](){
  handleRootCmd(5);
  });
  server.on("/7", [](){
  handleRootCmd(7);
  });  
  server.on("/6", [](){
  watsdoin=6;
  handleRootCmd(6);
  });
  server.on("/8", [](){
  handleRootCmd(8);
  });
  server.on("/9", [](){
  handleRootCmd(9);
  });
  server.on("/10", [](){watsdoin=10;handleRoot();
  });
  server.on("/11", [](){watsdoin=11;handleRoot();
  });
  server.onNotFound(handleNotFound);
  server.begin();
  delay(200);
  strip.begin();
  strip.show();
  delay(200);
  Serial.println("setup complete");
}

void loop(void) {
  
  theTime = millis();
  int diff = (theTime-lastTime1);
  String diffs =upCalc(diff);
  String theTimes =upCalc(theTime);
  
   Serial.print("timef");Serial.println(theTimes);
  Serial.print("diff");Serial.println(diffs);
  ArduinoOTA.handle();
 server.handleClient();
if (firstRun) {
Serial.println("1");
kitt();
    firstRun = false;
    secondRun = true;
  } else {
 if (theTime >= (lastTime1 + (stdDelaySec * 1000))) {
    runThePix();
    Serial.println("wats =");
    Serial.println(watsdoin);
    lastTime1=theTime;
    strip.clear();
  }
}
 yield();
}