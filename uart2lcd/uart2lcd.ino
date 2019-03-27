/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define TFT_CS     PA0
#define TFT_RST    PA2
/* maybe A0 instead on your board */
#define TFT_DC     PA1

// For 1.44" and 1.8" TFT with ST7735 (including HalloWing) use:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
uint32_t count = 0;
volatile uint32_t mode = 0;
char tmp = 0;
void setup(void) {
  pinMode(PC14, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PC14), change_display_mode, FALLING);
  Serial.begin(115200);
  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(true);
  tft.setRotation(tft.getRotation()+1);
}

void loop(){
  while(Serial.available()>0)
  {

    if(mode == 0) // BYTE mode
    {
      if(++count > 416)
      {
        count = 1;
        tft.fillScreen(ST77XX_BLACK);
        tft.setCursor(0, 0);
      }
      tmp = Serial.read();
      if(tmp >= 0x20 && tmp <= 0x7e)
        tft.write(tmp);
      else
        tft.print('.');
    }
    else // int mode
    {
      if(++count > 144)
      {
        count = 1;
        tft.fillScreen(ST77XX_BLACK);
        tft.setCursor(0, 0);
      }
      tmp = Serial.read();
      if(tmp == 0)
      {
        tft.print(tmp, HEX);
        tft.print(tmp, HEX);
      }
      else
        tft.print(tmp, HEX);
      if(!(count%9) && count != 0);
        //tft.print('\n');
      else
        tft.print(' ');
    }
  }
}

void change_display_mode()
{
  mode ^= 1;
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  count = 0;
}
