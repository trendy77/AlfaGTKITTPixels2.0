#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include <brzo_i2c.h> // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306Brzo.h"

#define PROGMEM ICACHE_RODATA_ATTR
///Declare a global string to be stored in flash.
static const char xyz[] PROGMEM = "This is a string stored in flash";


SSD1306Brzo  display(0x3c, 5, 4);
int DISPLAY_HEIGHT = 64;
int DISPLAY_WIDTH = 128;
boolean isWhite = false;
int bufferLines = 0;
int chars = 0;
const char *buffArr[] = {};

void drawLine(byte xs[], byte ys[]) {
  for (int16_t u = 0; u < sizeof(xs) - 1; u++)
  {
      display.drawLine(0, 0, xs[u], ys[u]);
      display.display();
      delay(10);
  }
}

// Adapted from Adafruit_SSD1306
void drawRect(void) {
  for (int16_t i = 0; i < DISPLAY_HEIGHT / 2; i += 2) {
    display.drawRect(i, i, DISPLAY_WIDTH - 2 * i, DISPLAY_HEIGHT - 2 * i);
    display.display();
    delay(10);
  }
}
void fillRect(void) {
  uint8_t color = 1;
  for (int16_t i = 0; i < DISPLAY_HEIGHT / 2; i += 3) {
    display.setColor((color % 2 == 0) ? BLACK : WHITE); // alternate colors
    display.fillRect(i, i, DISPLAY_WIDTH - i * 2, DISPLAY_HEIGHT - i * 2);
    display.display();
    delay(10);
    color++;
  }
  // Reset back to WHITE
  display.setColor(WHITE);
}

// Adapted from Adafruit_SSD1306
void drawCircle(void) {
  for (int16_t i = 0; i < DISPLAY_HEIGHT; i += 2) {
    display.drawCircle(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, i);
    display.display();
    delay(10);
  }
  delay(1000);
  display.clear();
  // This will draw the part of the circel in quadrant 1
  // Quadrants are numberd like this:
  //   0010 | 0001
  //  ------|-----
  //   0100 | 1000
  display.drawCircleQuads(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, DISPLAY_HEIGHT / 4, 0b00000001);
  display.display();
  delay(200);
  display.drawCircleQuads(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, DISPLAY_HEIGHT / 4, 0b00000011);
  display.display();
  delay(200);
  display.drawCircleQuads(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, DISPLAY_HEIGHT / 4, 0b00000111);
  display.display();
  delay(200);
  display.drawCircleQuads(DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, DISPLAY_HEIGHT / 4, 0b00001111);
  display.display();
}

void buffIt(const char toAdd[], boolean print){
    int nextAdd = chars;
    for (uint8_t i = 0; i < (sizeof(toAdd) - 1); i++){
        buffArr[chars]+=toAdd[i];
        chars++;
    }
    bufferLines++;
    if (!print)
    {
        return;
    }
    else
    {
        display.setLogBuffer(bufferLines, chars);
        for (uint8_t i = 0; i < bufferLines - 1; i++)
        {
            display.clear();
            display.println(buffArr[i]);
            display.drawLogBuffer(0, 0);
            display.display();
            delay(200);
        }
     
        bufferLines = 0;
        chars = 0;
    }
    return;
}

void loopSerialBuff(const char *serialstring){
/*serialstring.(toCharArray, numb = 0;
 while (Serial.available())
 {
     character = Serial.read();
     content[numb] = character;
     numb++;
  }
*/
}


void setupOled(void) {
  display.init();
  // display.flipScreenVertically();
  display.setContrast(255);
  buffIt("hey hey!", false);
  buffIt("no error?!",0);
  drawRect();
  delay(1000);
  fillRect();
  delay(1000);
  buffIt("HELLO MOTHAFUCKIN WORLD!", true);
  delay(1000);
  buffIt("setup COMPLETE", true);
}

