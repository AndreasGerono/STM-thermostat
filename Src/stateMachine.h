//
//library for managing time tasks/events
//

//for functions with parameter use events, task can only manage void functions!!
//events use with 'if' function in main loop

#include "stm32l0xx_hal.h"


typedef void (*taskFuncptr)(); 

typedef struct{
	uint32_t interval;
	uint32_t lastTick;
	taskFuncptr taskFunc;
} Task;

typedef struct{
	uint32_t interval;
	uint32_t lastTick;
} Event;



void task_reset(Task *);
void task_stop(Task *);
void task_check(Task*);


void event_reset(Event *);
void event_stop(Event *);

_Bool event_State(Event*);

Task task_make(uint32_t interval_ms, void (*func)() );
Event event_make(uint32_t interval_ms);



