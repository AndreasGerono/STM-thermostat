//
//SSD1306 Library with i2c transmission
//

#include "stm32l0xx_hal.h"


#define SSD1306_I2C &hi2c1
#define SSD1306_I2C_ADDR  0x78

#define SSD1306_128_64
//#define SSD1306_128_32

#define SSD1306_WIDTH 128
#if defined SSD1306_128_32
  #define SSD1306_HEIGHT 32
#elif defined SSD1306_128_64
  #define SSD1306_HEIGHT 64
#endif


//SSD1306 command set
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR 0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

//Colors

#define  COLOR_WHITE 1
#define  COLOR_BLACK 0


void ssd1306_clear();
void ssd1306_fill(uint8_t color);

void ssd1306_drawPixel(uint8_t x, uint8_t y, uint8_t color);
void ssd1306_init();
void ssd1306_writeCommand(uint8_t);
void ssd1306_display();




//
//End of file
//