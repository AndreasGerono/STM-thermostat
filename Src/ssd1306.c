#include "ssd1306.h"
#include "i2c.h"
#include "string.h"
#include "stateMachine.h"
#include "stdlib.h"

#define SSD1306_CMD 0x0
#define SSD1306_DATA 0x40
#define TIMEOUT 100

#define BUF_SIZE (SSD1306_WIDTH * SSD1306_HEIGHT / 8)

static uint8_t buffer[BUF_SIZE];
static uint8_t bufferToSend[BUF_SIZE];


void ssd1306_clear() {
  memset(buffer, 0, BUF_SIZE);
}
void ssd1306_fill(uint8_t color) {
  color *= 255;
  memset(buffer, color, BUF_SIZE);
}

void ssd1306_drawPixel(uint8_t x, uint8_t y, uint8_t color) {
  if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
    return;
  if (color == COLOR_WHITE)
    buffer[x + (y / 8) * SSD1306_WIDTH] |= ((1 << (y % 8)));
  if (color == COLOR_BLACK)
    buffer[x + (y / 8) * SSD1306_WIDTH] &= ~((1 << (y % 8)));
}

void ssd1306_display() {
  
  uint8_t controlData = SSD1306_DATA;
  memcpy_fast(bufferToSend,buffer,BUF_SIZE);
  HAL_I2C_Mem_Write_DMA(SSD1306_I2C, SSD1306_I2C_ADDR, controlData, I2C_MEMADD_SIZE_8BIT, bufferToSend, BUF_SIZE);
}

void ssd1306_writeCommand(uint8_t cmd) {
  uint8_t controlData = SSD1306_CMD;
  HAL_I2C_Mem_Write(SSD1306_I2C, SSD1306_I2C_ADDR, controlData, I2C_MEMADD_SIZE_8BIT, &cmd, 1, TIMEOUT);
}

void ssd1306_init() {
  ssd1306_writeCommand(SSD1306_DISPLAYOFF);
  ssd1306_writeCommand(SSD1306_SETDISPLAYCLOCKDIV);
  ssd1306_writeCommand(0xf0);                     // Refresh time
  ssd1306_writeCommand(SSD1306_SETMULTIPLEX);     // 0xA8
  ssd1306_writeCommand(SSD1306_HEIGHT - 1);
  ssd1306_writeCommand(SSD1306_SETDISPLAYOFFSET);
  ssd1306_writeCommand(0x0);                       // no offset
  ssd1306_writeCommand(SSD1306_SETSTARTLINE | 0x0);
  ssd1306_writeCommand(SSD1306_CHARGEPUMP);
  ssd1306_writeCommand(0x14);                     //chargepump ON

  ssd1306_writeCommand(SSD1306_MEMORYMODE);
  ssd1306_writeCommand(0x00);
  ssd1306_writeCommand(SSD1306_SEGREMAP | 0x1);
  ssd1306_writeCommand(SSD1306_COMSCANDEC);

#if defined SSD1306_128_32
  ssd1306_writeCommand(SSD1306_SETCOMPINS);
  ssd1306_writeCommand(0x02);
  ssd1306_writeCommand(SSD1306_SETCONTRAST);
  ssd1306_writeCommand(0x8F);
#elif defined SSD1306_128_64
  ssd1306_writeCommand(SSD1306_SETCOMPINS);
  ssd1306_writeCommand(0x12);
  ssd1306_writeCommand(SSD1306_SETCONTRAST);
  ssd1306_writeCommand(0xCF);
#endif
  ssd1306_writeCommand(SSD1306_SETPRECHARGE);
  ssd1306_writeCommand(0xF1);
  ssd1306_writeCommand(SSD1306_COLUMNADDR);
  ssd1306_writeCommand(0);
  ssd1306_writeCommand(127);
  ssd1306_writeCommand(SSD1306_PAGEADDR);
  ssd1306_writeCommand(0);
  ssd1306_writeCommand(7);
  ssd1306_writeCommand(SSD1306_DISPLAYALLON_RESUME);
  ssd1306_writeCommand(SSD1306_NORMALDISPLAY);
  ssd1306_writeCommand(SSD1306_DISPLAYON);
  
}