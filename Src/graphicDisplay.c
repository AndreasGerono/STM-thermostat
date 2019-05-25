//
//Version without font Size!!! - much faster
//
#include "graphicDisplay.h"
#include "ssd1306.h"


#define  SPACE 32

static Font *currentFont;

typedef struct {
  uint8_t x;
  uint8_t y;
} Cursor;

static Cursor cursor;

void display_clear(){
  ssd1306_clear();
  display_setCursor(0,0);
}
void display_fill(){
  
}

void display_setCursor(uint8_t x, uint8_t y) {
  cursor.x = x *  (1+ currentFont->width);
  cursor.y = y * (1 + currentFont->height);
}

void display_setFont(Font *font) {
  currentFont = font;
}

void display_writeText(char *text) {
  while (*text) {
    if (cursor.x >= SSD1306_WIDTH -  currentFont->width){
      cursor.y += (1+ currentFont->height);
      cursor.x = 0;
      if (*text == SPACE || *text == '\n')
        text++;
    }
    else if (*text == '\n') {
      cursor.y += (1+ currentFont->height);
      cursor.x = 0;
      text++;
    } 
    else {
      display_drawChar(cursor.x, cursor.y, *text++);
      cursor.x += (1+ currentFont->width);
    }
  }
}

void display_drawChar(uint8_t x, uint8_t y, char c) {
  if ((x >= SSD1306_WIDTH) ||
      (y >= SSD1306_HEIGHT) ||
      ((x + (currentFont->width + 1)) < 0) ||
      ((y + 8 * 1 - 1) < 0))
    return;

  uint8_t line;
  x+=1; 
  y+=1;  // use with fonts close to left bg
  
  display_drawHLine(x - 1, x + (currentFont->width), y-1, currentFont->bgColor); //use with font without upper border

  for (int8_t i = -1; i <= currentFont->width; i++) {
    if (i == currentFont->width || i == -1)
      line = 0x0;
    else
      line = currentFont->data[c * currentFont->width + i];
    for (int8_t j = 0; j <= currentFont->height; j++) {
      if (line & 0x1)
          ssd1306_drawPixel(x+i, y+j, currentFont->color);
      else if (currentFont->color != currentFont->bgColor)
          ssd1306_drawPixel(x+i, y+j, currentFont->bgColor);
      line >>= 1;
    }
  }
}

void display_drawBitmap(uint8_t x, uint8_t y, Bitmap bitmap) {
  uint16_t byteWidth = (bitmap.w + 7) / 8;

  for (uint16_t j = 0; j < bitmap.h; j++) {
    for (uint16_t i = 0; i < bitmap.w; i++) {
      if (bitmap.data[j * byteWidth + i / 8] & (128 >> (i % 8))) {
        ssd1306_drawPixel(x + i, y + j, bitmap.color);
      } else
        ssd1306_drawPixel(x + i, y + j, bitmap.bgColor);
    }
  }
}
void display_fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  for (uint8_t i = 0; i <= h; i++)
    display_drawHLine(x, x + w, y + i, color);
}

void display_drawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t color) {
  display_drawHLine(x, x + w - 1, y, color);
  display_drawHLine(x, x + w - 1, y + h - 1, color);
  display_drawVLine(y, y + h - 1, x, color);
  display_drawVLine(y, y + h - 1, x + w - 1, color);
}

void display_drawHLine(uint8_t x0, uint8_t x1, uint8_t y, uint8_t color) {
  while (x0 <= x1)
    ssd1306_drawPixel(x0, y, color), x0++;
}

void display_drawVLine(uint8_t y0, uint8_t y1, uint8_t x, uint8_t color) {
  while (y0 <= y1) 
    ssd1306_drawPixel(x, y0, color), y0++;
}