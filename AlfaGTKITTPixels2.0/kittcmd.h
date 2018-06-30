/**
* NEOPIXEL KITT BETA2 - PRODUCTION
**/
#include <Adafruit_NeoPixel.h>
#define PIN 4
#define NUMPIXELS 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 35;

void clearStrip()
{
  for (int i = 0; i < NUMPIXELS; i++)
  {
    strip.setPixelColor(i, 0x000000);
    strip.show();
  }
}
uint32_t dimColor(uint32_t color, uint16_t wid)
{
  return (((color & 0xFF0000) / wid) & 0xFF0000) + (((color & 0x00FF00) / wid) & 0x00FF00) + (((color & 0x0000FF) / wid) & 0x0000FF);
}
uint32_t colorWheel(byte WheelPos)
{
  byte state = WheelPos / 21;
  switch (state)
  {
  case 0:
    return strip.Color(255, 0, 255 - ((((WheelPos % 21) + 1) * 6) + 127));
    break;
  case 1:
    return strip.Color(255, ((WheelPos % 21) + 1) * 6, 0);
    break;
  case 2:
    return strip.Color(255, (((WheelPos % 21) + 1) * 6) + 127, 0);
    break;
  case 3:
    return strip.Color(255 - (((WheelPos % 21) + 1) * 6), 255, 0);
    break;
  case 4:
    return strip.Color(255 - (((WheelPos % 21) + 1) * 6) + 127, 255, 0);
    break;
  case 5:
    return strip.Color(0, 255, ((WheelPos % 21) + 1) * 6);
    break;
  case 6:
    return strip.Color(0, 255, (((WheelPos % 21) + 1) * 6) + 127);
    break;
  case 7:
    return strip.Color(0, 255 - (((WheelPos % 21) + 1) * 6), 255);
    break;
  case 8:
    return strip.Color(0, 255 - ((((WheelPos % 21) + 1) * 6) + 127), 255);
    break;
  case 9:
    return strip.Color(((WheelPos % 21) + 1) * 6, 0, 255);
    break;
  case 10:
    return strip.Color((((WheelPos % 21) + 1) * 6) + 127, 0, 255);
    break;
  case 11:
    return strip.Color(255, 0, 255 - (((WheelPos % 21) + 1) * 6));
    break;
  default:
    return strip.Color(0, 0, 0);
    break;
  }
}

void knightRider(uint16_t cyc, uint16_t spd, uint16_t wid, uint32_t color)
{
  uint32_t old_val[NUMPIXELS]; // up to 256 lights!
  for (int i = 0; i < cyc; i++)
  {
    for (int count = 1; count < NUMPIXELS; count++)
    {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for (int x = count; x > 0; x--)
      {
        old_val[x - 1] = dimColor(old_val[x - 1], wid);
        strip.setPixelColor(x - 1, old_val[x - 1]);
      }
      strip.show();
      delay(spd);
    }
    for (int count = NUMPIXELS - 1; count >= 0; count--)
    {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for (int x = count; x <= NUMPIXELS; x++)
      {
        old_val[x - 1] = dimColor(old_val[x - 1], wid);
        strip.setPixelColor(x + 1, old_val[x + 1]);
      }
      strip.show();
      delay(spd);
    }
    server.handleClient();
  }
  clearStrip();
}
void kitt()
{
  int t = 30;
  knightRider(3, t, 4, 0xFF1000); // Cycles, Speed, Width, RGB Color (original orange-red)
  knightRider(3, t, 3, 0xFF00FF); // Cycles, Speed, Width, RGB Color (purple)
  knightRider(3, t, 2, 0x0000FF); // Cycles, Speed, Width, RGB Color (blue)
  knightRider(3, t, 5, 0xFF0000); // Cycles, Speed, Width, RGB Color (red)
  knightRider(3, t, 4, 0x00FF00); // Cycles, Speed, Width, RGB Color (green)
  knightRider(3, t, 4, 0xFFFF00); // Cycles, Speed, Width, RGB Color (yellow)
  knightRider(3, t, 4, 0x00FFFF); // Cycles, Speed, Width, RGB Color (cyan)
  knightRider(2, t, 2, 0xFFFFFF); // Cycles, Speed, Width, RGB Color (white)
  clearStrip();
}
void copMode()
{
  int del = 20;
  for (int cy = 1; cy <= 5; cy++)
  {
    // set to 5 cycles...
    knightRider(1, del, 5, 0xFF0000); // red
    knightRider(1, del, 5, 0x0000FF); // blue
    del = (del - 2);
  }
}

// Input a value 0 to 255 to get a color value.  The colours are a transition r - g - b - back to r.
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

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
  server.handleClient();
}
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<5; j++) {  //do 5 cycles of chasing
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
    server.handleClient();
  }
}

void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  server.handleClient();
  }
}
void rainbow() {
  for (byte j = 0; j<252; j += 7) {
    knightRider(1, 16, 2, colorWheel(j)); // Cycles, Speed, Width, RGB Color
  }
  clearStrip();
}
void copMode2(long delayz) {
 long dastart=millis();
long timeCh=(millis()-dastart);
  while (timeCh<=(delayz*1000)){
 colorWipe(strip.Color(255,0,0), 50); 
 delay(250);
 server.handleClient();
 colorWipe(strip.Color(0,0,127), 50); // Blue
 delay(250);
}
}

uint8_t red(uint32_t c) {
  return (c >> 8);
}
uint8_t green(uint32_t c) {
  return (c >> 16);
}
uint8_t blue(uint32_t c) {
  return (c);
}


/////////////////////////////////////////
//////////////////// NEW JULY LIGHTS!

void pulseWhite(uint8_t wait) {
  for(int j = 0; j < 256 ; j++){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, j ) );
        }
        delay(wait);
        strip.show();
        j=(j+2);
      }
  for(int j = 255; j >= 0 ; j--){
      for(uint16_t i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, strip.Color(0,0,0, j ) );
        }
        delay(wait);
        strip.show();
        j=(j-2);
      }
}

void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;
  for(int k = 0 ; k < rainbowLoops ; k ++){
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel
      for(int i=0; i< strip.numPixels(); i++) {
        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);
        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);
        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );
      }
      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
          fadeVal++;
      }
      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
          fadeVal--;
      }
       strip.show();
        delay(wait);
    } 
  }
  delay(500);
 for(int k = 0 ; k < whiteLoops ; k ++){
    for(int j = 0; j < 256 ; j++){
        for(uint16_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,0, j) );
          }
        j=(j+2);
        strip.show();
        }
           delay(2000);
    for(int j = 255; j >= 0 ; j--){
        for(uint16_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, strip.Color(0,0,0, j ) );
          }
          strip.show();
          j=(j-2);
        }
  }
  delay(500);
}

void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength ) {
  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;
  int head = whiteLength - 1;
  int tail = 0;
  int loops = 3;
  int loopNum = 0;
  static unsigned long lastTime = 0;
  while(true){
    for(int j=0; j<256; j++) {
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        if((i >= tail && i <= head) || (tail > head && i >= tail) || (tail > head && i <= head) ){
          strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
        }
        else{
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }  
      }
      if(millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if(head == strip.numPixels()){
          loopNum++;
        }
        lastTime = millis();
      }
      if(loopNum == loops) return;  
      head%=strip.numPixels();
      tail%=strip.numPixels();
        strip.show();
        delay(wait);
    }
  }
}
void fullWhite() {
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, strip.Color(0,0,0, 255 ) );
    }
      strip.show();
}
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256 * 5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void newLoop() {
  whiteOverRainbow(20,75,5);  
  pulseWhite(10);
  rainbow(30); 
  rainbowFade2White(3,3,1);
  fullWhite();
  clearStrip();
  kitt();
  delay(500);
}

