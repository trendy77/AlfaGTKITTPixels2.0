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
#include <ArduinoOTA.h>
#include <aREST.h>

// Create aREST instance
aREST rest = aREST();
#define LISTEN_PORT           80
WiFiServer server(LISTEN_PORT);

#define PIN 4
#define NUMPIXELS 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 500;

String modeS = "kitt";
int speedy = 25;
int cycles = 5;
int width = 4;
int stdDelaySec = 30;
String colour = "red";
int red, green, blue = 200;
int watsdoin = 1;

const char ssid[] = "Northern Frontier Interwebs";
const char pass[] = "num4jkha8nnk"; 
IPAddress subnet(255, 255, 255, 0);
IPAddress gateway(10, 0, 77, 100);
//WiFiClient client;
#define HOST_NAME "alfagt"
IPAddress ip(10, 0, 77, 111);

const char ssid3[] = "AlfaRomeoGT";
const char pass3[] = "turismoGT";
bool firstRun, secondRun, off = false;
const char ssid2[] = "AndroidAP";
const char pass2[] = "tttttttt";
long theTime, lastTime1 = 0;

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

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}void clearStrip() {
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
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}


void kitt() {
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
void rainbow() {
  for (byte j = 0; j<252; j += 7) {
    knightRider(1, 16, 2, colorWheel(j)); // Cycles, Speed, Width, RGB Color
  }
  clearStrip();
}
void copMode() {
 colorWipe(strip.Color(255, 0, 0), 50); 
  delay(1000);
    theaterChase(strip.Color(  0,   0, 127), 50); // Blue
  delay(1000);
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();
      delay(delayval);
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
void runThePix() {
switch (watsdoin){    // 1st column
  case '1': //  STANDARD KITT
     knightRider(cycles, speedy, width, 0xFF1000); // (original orange-red)
    break;
  case '4': copMode();
    break;
  case '7':     rainbow();       break;
    
  case '2':  knightRider(cycles, speedy, width, 0x00FF00);      // greenbreak;     // green
  case '5': knightRider(cycles, speedy, width, 0xFFFF00);     break;       // yellow
  case '8':  
    knightRider(cycles, speedy, width, 0x0000FF);      // blue         
  break;     // blue
   // 3rd column delay timing changes
  case '3': stdDelaySec = 10;   break;
  case '6': stdDelaySec = 30;   break;
  case '9': stdDelaySec = 60;   break;
    // ON / OFF
  case '0':   off = true; clearStrip();    break;
  }
}

int kittControl(String Command) {
  int cmdd = Command.toInt();
  watsdoin=cmdd;
  return cmdd;
}

void setup(void) {
  Serial.begin(115200);
  stdDelaySec = 10;
  firstRun = true;
  watsdoin = 1;
   rest.variable("watsdoin", &watsdoin);
//  rest.variable("speedy", &speedy);
 // rest.variable("width", &width);
//  rest.variable("red", &red);
//  rest.variable("blue", &blue);
//  rest.variable("green", &green);
  rest.variable("delaySecs", &stdDelaySec);
  rest.function("kittControl", kittControl);
  
  rest.set_id("1");
  rest.set_name("node1");

  WiFi.config(ip, gateway, subnet);
  String hostname(HOST_NAME);
  WiFi.hostname(hostname);
  WiFi.begin(ssid, pass);
   int n = WiFi.scanNetworks();
  //Serial.println("scan done");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected"); Serial.println(ssid); Serial.print("IP address: ");    Serial.println(WiFi.localIP());
  for (int i = 0; i < n; ++i) {
   String tryWi = WiFi.SSID(i);
   if (tryWi == ssid) {
  WiFi.begin(ssid, pass);
   }
   else if (tryWi == ssid2) {
    WiFi.begin(ssid2, pass2);
   }
  else if (tryWi == ssid3) {
      WiFi.begin(ssid3, pass3);
   }
  }
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
  server.begin();
  strip.begin();
  strip.show();
  Serial.println("ALFAPixels server started");
 
 delay(500);
  kitt();
}

void loop(void) {
  ArduinoOTA.handle();
    WiFiClient client = server.available();
  theTime = millis();
  if(client.available()){
  rest.handle(client);
  }
  if (firstRun == 1) {
watsdoin=7;
firstRun = false;
    secondRun = true;
  } if (secondRun == 1) {
   watsdoin=4;
    secondRun = false;
  }
watsdoin=1;
  if (theTime >= (lastTime1 + (stdDelaySec * 1000))) {
    lastTime1=theTime;
    runThePix();
    strip.clear();
  }
 yield();
}
