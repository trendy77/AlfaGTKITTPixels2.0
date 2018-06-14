/**
* NEOPIXEL KITT BETA - PRODUCTION
**/
#include <Adafruit_NeoPixel.h>

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

void kitt2() {
  knightRider(3, 12, 4, 0xFF1000); // Cycles, Speed, Width, RGB Color (original orange-red)
  knightRider(3, 17, 4, 0xFF00FF); // Cycles, Speed, Width, RGB Color (purple)
  knightRider(3, 22, 4, 0x0000FF); // Cycles, Speed, Width, RGB Color (blue)
  knightRider(3, 27, 5, 0xFF0000); // Cycles, Speed, Width, RGB Color (red)
  knightRider(3, 32, 6, 0x00FF00); // Cycles, Speed, Width, RGB Color (green)
  knightRider(3, 32, 4, 0xFFFF00); // Cycles, Speed, Width, RGB Color (yellow)
  knightRider(3, 32, 5, 0x00FFFF); // Cycles, Speed, Width, RGB Color (cyan)
  knightRider(3, 32, 3, 0xFFFFFF); // Cycles, Speed, Width, RGB Color (white)
  clearStrip();
  delay(2000);
}
void rainbow() {
  for (byte j = 0; j<252; j += 7) {
    knightRider(1, 16, 2, colorWheel(j)); // Cycles, Speed, Width, RGB Color
  }
  clearStrip();
}
void copMode2() {
 colorWipe(strip.Color(255, 0, 0), 50); 
  delay(1000);
    theaterChase(strip.Color(  0,   0, 127), 50); // Blue
  delay(1000);
}

void knightRider2(uint16_t cyc, uint16_t spd, uint16_t wid, uint32_t color) {
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

