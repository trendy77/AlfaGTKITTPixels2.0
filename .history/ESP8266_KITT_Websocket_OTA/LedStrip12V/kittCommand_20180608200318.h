#define REFRESH_RAINBOW 500
#define WAIT_RAINBOW 10000
#define SSID_ME "AndroidAP"
#define PW_ME "tttttttt"
#define HOST_ME "alfagt"

#ifndef DEBUGGING
#define DEBUGGING(...)
#endif
#ifndef DEBUGGING_L
#define DEBUGGING_L(...)
#endif

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
 
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <Arduino.h>
#include <WebSocketsServer.h>
#include <Hash.h>

// neopixels
#define PIN 4
#define NUMPIXELS 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 25;


//Globals
WebSocketsServer webSocket = WebSocketsServer(81);
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
const char* host = HOST_ME;
const char* ssid     = SSID_ME;
const char* password = PW_ME;
unsigned long lastTimeHost = 0;
unsigned long lastTimeRefresh = 0;
unsigned long lastTimeRefreshRainbow = 0;
boolean rainbowFlag = 1;
int RGB[3];
int cnt = 0;
int rainbowDelay = REFRESH_RAINBOW;

//Wheel - return HUE[iii] in RGB
void Wheel(int WheelPos, int* RGB) {
  WheelPos = WheelPos % 256;

  if (WheelPos < 85) {
    RGB[0] = WheelPos * 3;
    RGB[1] = 255 - WheelPos * 3;
    RGB[2] = 0;
  }
  else if (WheelPos < 170) {
    WheelPos -= 85;
    RGB[2] = WheelPos * 3;
    RGB[0] = 255 - WheelPos * 3;
    RGB[1] = 0;
  }
  else if (WheelPos < 255) {
    WheelPos -= 170;
    RGB[1] = WheelPos * 3;
    RGB[2] = 255 - WheelPos * 3;
    RGB[0] = 0;

  }
  else
  {
    WheelPos -= 255;
    RGB[0] = WheelPos * 3;
    RGB[1] = 255 - WheelPos * 3;
    RGB[2] = 0;
  }
}

uint32_t dimColor(uint32_t color, uint16_t wid) {
  return (((color & 0xFF0000) / wid) & 0xFF0000) + (((color & 0x00FF00) / wid) & 0x00FF00) + (((color & 0x0000FF) / wid) & 0x0000FF);
}

// Write wheel to leds
unit32_t writeWheel(int WheelPos, int* RGB) {
  Wheel(WheelPos, RGB);
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

// WebSOcket Events
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED:
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
      }
      break;
      
    case WStype_TEXT:
      {
        lastTimeRefresh = millis();

        String text = String((char *) &payload[0]);
        if (text == "LED") {
          digitalWrite(13, HIGH);
          delay(500);
          digitalWrite(13, LOW);
          Serial.println("led just lit");
          webSocket.sendTXT(num, "led just lit", length);
        }

        if (text.startsWith("w")) {
          String xVal = (text.substring(text.indexOf("watsdoin") + 1, text.length()));
          int xInt = xVal.toInt();
          analogWrite(REDPIN, xInt);
          DEBUGGING(xVal);
        }

        if (text.startsWith("d")) {
          String yVal = (text.substring(text.indexOf("delay") + 1, text.length()));
          int yInt = yVal.toInt();
          stdDelaySec=yInt; 
          DEBUGGING(yVal);
        }

        if (text.startsWith("z")) {
          String zVal = (text.substring(text.indexOf("z") + 1, text.length()));
          int zInt = zVal.toInt();
          analogWrite(BLUEPIN, zInt);
          DEBUGGING(zVal);
        }
        if (text.startsWith("t")) {
          String tVal = (text.substring(text.indexOf("t") + 1, text.length()));
          rainbowDelay = tVal.toInt();
          lastTimeRefreshRainbow = 0;
          lastTimeRefresh = 0;
          DEBUGGING(tVal);        
        }        
        if (text == "RESET") {
          rainbowFlag = 0;
          clearStrip();
          DEBUGGING("reset");

        }
        if (text == "RAINBOW") {
          rainbowFlag = 1;
          lastTimeRefreshRainbow = 0;
          lastTimeRefresh = 0;
       /*   for (int iii = 0; iii < 256; iii++) {
            writeWheel(iii, RGB);
            delay(10);
          }*/
          DEBUGGING("rainbow");
        }
      }
      break;

    case WStype_BIN:
      hexdump(payload, length);
      // echo data back to browser
      webSocket.sendBIN(num, payload, length);
      break;
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

// Wifi Connection
void WifiConnect() {

 // WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  DEBUGGING("Connected");
  DEBUGGING(WiFi.localIP());

}

// WebSocket Connection
void WebSocketConnect() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

// MDNS 
void MDNSConnect() {
  if (!MDNS.begin(host)) {
   DEBUGGING("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  DEBUGGING("mDNS responder started");
  MDNS.addService("ws", "tcp", 81);
  MDNS.addService("http", "tcp", 80);
}

// HTTP updater connection
void HTTPUpdateConnect() {
  httpUpdater.setup(&httpServer);
  httpServer.begin();
  DEBUGGING_L("HTTPUpdateServer ready! Open http://");
  DEBUGGING_L(host);
  DEBUGGING(".local/update in your browser\n");
}


