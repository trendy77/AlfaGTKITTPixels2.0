/**
* NEOPIXEL KITT BETA2 - PRODUCTION
**/
#include <Adafruit_NeoPixel.h>
#define PIN 4
#define NUMPIXELS 16
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 25;


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
        old_val[x - 1] = dimColor(old_val[x - 1], wid);
        strip.setPixelColor(x + 1, old_val[x + 1]);
      }
      strip.show();
      delay(spd);
    }
  }
  clearStrip();
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