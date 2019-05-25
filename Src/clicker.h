
#include "stateMachine.h"
#include "stm32l0xx_hal.h"


typedef void (*clickFuncptr)(); 

typedef struct {
  uint16_t pin;
  GPIO_TypeDef *gpio;
  _Bool debounceFlag;
  Event clickEvent;
  uint8_t timing;
  clickFuncptr clickFunc;
} Button;


Button clicker_make(GPIO_TypeDef *gpio, uint16_t pin, void (*func)());
void clicker_check(Button *button);