#include "ds3231.h"
#include "i2c.h"
#include "stm32l0xx_hal.h"
#include "string.h"

static const char *weekDays[] = {
    "No", " Mon", " Tue", " Wed", " Thu" , " Fri", " Sat", " Sun"};

const char *ds3221_GetDayName(uint8_t day){
  return weekDays[day];
}

void ds3231_InitializeTime(Time *time){
  time->Hour.maxValue = MAX_HOURS;
  time->Minute.maxValue = MAX_MINUTES;
  time->Second.maxValue = MAX_SECONDS;
}

void ds3231_InitializeDate(Date *date){
  date->Year.maxValue = MAX_YEAR;
  date->Month.maxValue = MAX_MONTH;
  date->Day.maxValue = MAX_DAYS;
  date->WeekDay.maxValue = MAX_WEEKDAY;
}

void ds3231_PrintTime(Time *time, char *name) {
  sprintf(name, "%02u:%02u:%02u", time->Hour.value, time->Minute.value, time->Second.value);
}

void ds3231_PrintDate(Date *date, char *name) {
  sprintf(name, "%02u-%02u-%u", date->Day.value, date->Month.value, date->Year.value);
  strcat(name, weekDays[date->WeekDay.value]);
}

void ds3231_GetTime(Time *time) {
  time->Second.value = bcd2dec(ds3231_Read(SecondsReg) & SECONDS_MASK);
  time->Minute.value = bcd2dec(ds3231_Read(MinutesReg) & MINUTES_MASK);
  time->Hour.value = bcd2dec(ds3231_Read(HoursReg) & HOURS_MASK);
}

void ds3231_SetTime(Time *time) {
  ds3231_Write(SecondsReg, dec2bcd(time->Second.value));
  ds3231_Write(MinutesReg, dec2bcd(time->Minute.value));
  ds3231_Write(HoursReg, dec2bcd(time->Hour.value));
}

void ds3231_GetDate(Date *date) {
  date->Day.value = bcd2dec(ds3231_Read(DayReg) & DAY_MASK);
  date->Year.value = bcd2dec(ds3231_Read(YearReg));
  date->Month.value = bcd2dec(ds3231_Read(MonthReg) & MONTH_MASK);
  date->WeekDay.value = bcd2dec(ds3231_Read(WeekDayReg) & WEEKDAY_MASK);
}

void ds3231_SetDate(Date *date) {
  ds3231_Write(DayReg, dec2bcd(date->Day.value));
  ds3231_Write(YearReg, dec2bcd(date->Year.value));
  ds3231_Write(MonthReg, dec2bcd(date->Month.value));
  ds3231_Write(WeekDayReg, dec2bcd(date->WeekDay.value));
}

void ds3231_IncrementTimeElement(uint16_t addr, Values * time){
  (time->value)++;
  (time->value) %= (time->maxValue);
  ds3231_Write(addr, dec2bcd(time->value));
}

void ds3231_IncrementDateElement(uint16_t addr, Values * date){
  date->value = (date->value % date->maxValue) + 1;
  ds3231_Write(addr, dec2bcd(date->value));
}

void ds3231_Write(uint16_t addr, uint8_t data) {
  HAL_I2C_Mem_Write(DS3231_I2C, DS3231_ADDR, addr, SIZE, &data, SIZE, TIMEOUT);
    while(HAL_I2C_IsDeviceReady(DS3231_I2C, DS3231_ADDR, 1, HAL_MAX_DELAY));
}

uint8_t ds3231_Read(uint16_t addr) {
  uint8_t data;
  HAL_I2C_Mem_Read(DS3231_I2C, DS3231_ADDR, addr, 1, &data, SIZE, TIMEOUT);
  return data;
}


uint8_t bcd2dec(uint8_t bcd) {
  return ((((bcd) >> 4) & 0x0F) * 10) + ((bcd)&0x0F);
}

uint8_t dec2bcd(uint8_t dec) {
  return ((dec / 10) << 4) | (dec % 10);
}