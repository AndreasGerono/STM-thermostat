#include "thermostat.h"
#include "adc.h"
#include "ds3231.h"
#include "memoryI2C.h"
#include "stm32l0xx_hal.h"
#include "string.h"
#include "tim.h"
#include "usart.h"

#define TEMPSIZE 6
#define TEMPOFFSET 6


static const char degreeC[] = {128, 'C'};

static uint32_t adcValue;

uint16_t temp_GetMeasuredTemp() {
    return ((adcValue * ADCREF_MV) / ADCMAX);
}

uint8_t temp_GetDayTemp() {
  uint16_t addr = memory_GetAddress(temp.dayIndex, 1, TEMP_SECTION);
  return memory_ReadByte(addr);
}

uint8_t temp_GetNightTemp() {
  uint16_t addr = memory_GetAddress(temp.dayIndex, 0, TEMP_SECTION);
  return memory_ReadByte(addr);
}

_Bool temp_GetSelectedDayNight() {
  uint16_t addr = memory_GetAddress(temp.dayIndex, temp.hourIndex, DAYNIGHT_SECTION);
  return memory_ReadByte(addr);
}

_Bool temp_GetSetDayNight() {
  uint16_t addr = memory_GetAddress(date.WeekDay.value, time.Hour.value, DAYNIGHT_SECTION);
  return memory_ReadByte(addr);
}

uint8_t temp_GetSetTemp() {
  _Bool dayNight = temp_GetSetDayNight();
  uint16_t addr = memory_GetAddress(date.WeekDay.value, dayNight, TEMP_SECTION);
  return memory_ReadByte(addr);
}

void temp_ToggleDayNight() {
  _Bool state = !temp_GetSelectedDayNight();
  uint8_t addr = memory_GetAddress(temp.dayIndex, temp.hourIndex, DAYNIGHT_SECTION);
  memory_WriteByte(addr, state);
}

void temp_IncrementHourIndex() {
  ++temp.hourIndex;
  temp.hourIndex %= MAX_HOURS;
}

void temp_IncrementDayIndex() {
  temp.dayIndex = (temp.dayIndex % MAX_WEEKDAY) + 1;
}

void temp_IncrementDayTemp() {
  uint8_t setTemp = temp_GetDayTemp();
  uint16_t addr = memory_GetAddress(temp.dayIndex, 1, TEMP_SECTION);
  setTemp += 5;
  if (setTemp < TEMPMIN)
    setTemp = TEMPMIN;
  memory_WriteByte(addr, setTemp);
}
void temp_IncrementNightTemp() {
  uint8_t setTemp = temp_GetNightTemp();
  uint16_t addr = memory_GetAddress(temp.dayIndex, 0, TEMP_SECTION);
  setTemp += 5;
  if (setTemp < TEMPMIN)
    setTemp = TEMPMIN;
  memory_WriteByte(addr, setTemp);
}

void temp_setThermostatState() {
  uint16_t setTemp = temp_GetSetTemp();
  uint16_t measuredTemp = temp_GetMeasuredTemp();
  if ((setTemp - HYSTERESIS) >= measuredTemp)
    temp.state = SET;
  if ((measuredTemp - HYSTERESIS) >= setTemp)
    temp.state = RESET;
}

void temp_TransmitState() {
  uint8_t data = temp.state;
  HAL_UART_Transmit(RF_UART, &data, 1, TIMEOUT);
}

void temp_PrintTemp(char *name1, uint16_t temp, uint8_t offset) {
  char name2[TEMPSIZE];
  sprintf(name2, "%02u.%1u ", temp / 10, temp % 10);
  strncpy(name1 + offset, name2, TEMPSIZE);
  strncpy(name1 + offset + 4, degreeC, 2);
}

void temp_ResetIndexes() {
  temp.dayIndex = date.WeekDay.value;
  temp.hourIndex = time.Hour.value;
}

void temp_init() {
  HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED);
  HAL_TIM_Base_Start(&htim6);
  HAL_ADC_Start_DMA(&hadc, &adcValue, 1);
  temp_ResetIndexes();
}