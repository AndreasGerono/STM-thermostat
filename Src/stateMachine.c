#include "stateMachine.h"
#include "stm32l0xx_hal.h"


static volatile uint32_t currentTick;

void HAL_SYSTICK_Callback(void) {
  currentTick++;
}

void event_reset(Event *event) {
  event->lastTick = currentTick;
}

void event_stop(Event *event){
  event->lastTick = 0;
}

void task_reset(Task *task) {
  task->lastTick = currentTick;
}

void task_stop(Task *task) {
  task->lastTick = 0;
}

void task_check(Task *task) {
  if ((currentTick - task->lastTick >= task->interval) && task->lastTick) {
    task->lastTick = currentTick;
    task->taskFunc();
  }
}

_Bool event_State(Event *event) {
  if ((currentTick - event->lastTick >= event->interval) && event->lastTick) {
    event->lastTick = currentTick;
    return 1;
  } else
    return 0;
}

Task task_make(uint32_t interval_ms, void (*func)()) {
  Task task;
  task.interval = interval_ms;
  task.lastTick = currentTick;
  task.taskFunc = func;
  return task;
}

Event event_make(uint32_t interval_ms) {
  Event event;
  event.interval = interval_ms;
  event.lastTick = currentTick;
  return event;
}