#include "clicker.h"
#include "ssd1306.h"

#define DEFAULT_TIME 170
#define MINIMUM_TIME 50
#define DEBOUNCE_TIME 20
#define ACCELERATION 10

static void debounce(Button *btn) {
  if (btn->debounceFlag) {
    event_reset(&btn->clickEvent);
    btn->clickEvent.interval = DEBOUNCE_TIME;
    btn->debounceFlag = RESET;
  }
}

static void click(Button *btn) {
  if (event_State(&btn->clickEvent)) {
    btn->clickFunc();
    btn->clickEvent.interval = btn->timing;
    if ((btn->timing > MINIMUM_TIME) && ACCELERATION)
      btn->timing -= ACCELERATION;
  }
}

static void resetClicker(Button *btn) {
  btn->debounceFlag = SET;
  btn->timing = DEFAULT_TIME;
}

Button clicker_make(GPIO_TypeDef *gpio, uint16_t pin, void (*func)()) {
  Button btn;
  btn.pin = pin;
  btn.gpio = gpio;
  btn.debounceFlag = SET;
  btn.timing = DEFAULT_TIME;
  btn.clickFunc = func;
  btn.clickEvent = event_make(btn.timing);
  return btn;
}

void clicker_check(Button *button) {
  if (!HAL_GPIO_ReadPin(button->gpio, button->pin)) {
    debounce(button);
    click(button);
  } else
    resetClicker(button);
}