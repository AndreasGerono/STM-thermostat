#include "stm32l0xx_hal.h"
#include "font.h"



typedef struct {
 uint8_t w;
 uint8_t h;
 uint8_t color;
 uint8_t bgColor;
 const uint8_t *data;
} Bitmap;


void display_clear();
void display_fill();

void display_setFont(Font*);
void display_setCursor(uint8_t x, uint8_t y);

void display_writeText(char *text);
void display_drawChar(uint8_t x, uint8_t y, char c);
void display_drawBitmap(uint8_t x,uint8_t y, Bitmap bitmap);
void display_fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);

void display_drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color);
void display_drawVLine(uint8_t y0, uint8_t y1, uint8_t x, uint8_t color);
void display_drawHLine(uint8_t x0, uint8_t x1, uint8_t y, uint8_t color);