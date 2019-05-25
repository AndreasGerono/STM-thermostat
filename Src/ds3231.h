//configurate I2C in CubeMx with default settings

#include "i2c.h"
#include "main.h"
#include "stm32l0xx_hal.h"

#define DS3231_I2C &hi2c3   //define used I2C
#define DS3231_ADDR 0xd0 //ds3231 address shifted to left
#define TIMEOUT 100      //TIMEOUT to ERROR
#define SIZE 1           //Data size


//
//Registers MASK
//
#define HOURS_MASK 0x3f
#define SECONDS_MASK 0x7f
#define MINUTES_MASK 0x7f
#define WEEKDAY_MASK 0x7
#define DAY_MASK 0x3f
#define MONTH_MASK 0x1f
//
//Maximum Values
//

#define MAX_HOURS 24
#define MAX_MINUTES 60
#define MAX_SECONDS 60
#define MAX_MONTH 12
#define MAX_DAYS 31
#define MAX_YEAR 99
#define MAX_WEEKDAY 7

//
//Addres of registers in ds3231
//


enum registers {
  SecondsReg,
  MinutesReg,
  HoursReg,
  WeekDayReg,
  DayReg,
  MonthReg,
  YearReg,
  Alarm1SecReg,
  Alarm1MinReg,
  Alarm1HourReg,
  Alarm1DayDateReg,
  Alarm2MinReg,
  Alarm2HourReg,
  Alarm2DayDateReg,
  ControlReg,
  ControlStatusReg,
  TmpMsbReg,
  TmpLsbReg
};

enum WeekDays {
  Monday = 1,
  Thuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday
};

enum Months {
  January = 1,
  February,
  March,
  April,
  May,
  June,
  July,
  Septemper,
  August,
  October,
  November,
  December
};

typedef struct {
  uint8_t value;
  uint8_t maxValue;
} Values;

typedef struct { 
  Values Day;
  Values WeekDay;
  Values Month;
  Values Year;
} Date;

typedef struct {
  Values Second;
  Values Minute;
  Values Hour;
} Time;

extern Time time;
extern Date date;

void ds3231_InitializeTime(Time *time);
void ds3231_InitializeDate(Date *date);

void ds3231_GetTime(Time *);
void ds3231_GetDate(Date *);
const char *ds3221_GetDayName(uint8_t);

void ds3231_SetTime(Time *);
void ds3231_SetDate(Date *);

void ds3231_IncrementTimeElement(uint16_t, Values*);
void ds3231_IncrementDateElement(uint16_t, Values*);

void ds3231_PrintTime(Time *, char *);
void ds3231_PrintDate(Date *, char *);

void ds3231_Write(uint16_t, uint8_t); //write to register
uint8_t ds3231_Read(uint16_t);        //read from register

uint8_t bcd2dec(uint8_t);
uint8_t dec2bcd(uint8_t);
