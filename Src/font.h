#include "stm32l0xx_hal.h"


#define  FONTWIDTH 5
#define  FONTHEIGHT 8

typedef struct{
const char *data;
uint8_t width;
uint8_t height;
uint8_t size;
uint8_t color;
uint8_t bgColor;
} Font;

extern Font font1;
extern Font font2;
