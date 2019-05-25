//  Thermostat library with external i2c memory for temp. programs.
//
//

#include "stm32l0xx_hal.h"

#define  ADCMAX 4095
#define  ADCREF_MV 3300
#define  RF_UART &hlpuart1


// in 10*degree

#define  TEMPMIN 100
#define  HYSTERESIS 5


typedef struct{
  uint8_t dayIndex;
  uint8_t hourIndex;
  _Bool state;
} Thermostat;

extern Thermostat temp;

void temp_TransmitState();
void temp_PrintTemp(char *, uint16_t, uint8_t offset);

void temp_init();
void temp_IncrementHourIndex();
void temp_IncrementDayIndex();
void temp_IncrementDayTemp();
void temp_IncrementNightTemp();
void temp_ToggleDayNight();
void temp_ResetIndexes();
void temp_setThermostatState();

uint8_t temp_GetSetTemp();
_Bool temp_GetSetDayNight();

uint8_t temp_GetDayTemp();
uint8_t temp_GetNightTemp();
uint8_t temp_GetSetTemp();
_Bool temp_GetSelectedDayNight();


uint16_t temp_GetMeasuredTemp();






//
// End of file
//