//
//Menu with time and date settings
//

#include "ds3231.h"
#include "graphic_menu.h"
#include "string.h"
#include "thermostat.h"

#define POS_HOUR 6
#define POS_MINUTE 8
#define POS_WEEKDAY 9
#define POS_DAY 5
#define POS_MONTH 7
#define POS_YEAR 8

#define POS_TEMP_DAY 5
#define POS_TEMP_NIGHT 7
#define POS_DAYNIGHT 4

#define POS_HEATER_STATE 4

#define HEATERON {12,' ','\0'}
#define HEATEROFF {15,' ','\0'}

Time time;
Date date;
Thermostat temp;


char timeName[MENU_COLUMNS];
char hourName[MENU_COLUMNS] = "Hour: ";
char minuteName[MENU_COLUMNS] = "Minute: ";

char dateName[MENU_COLUMNS];
char dayName[MENU_COLUMNS] = "Day: ";
char weekdayName[MENU_COLUMNS] = "Weekday: ";
char monthName[MENU_COLUMNS] = "Month: ";
char yearName[MENU_COLUMNS] = "Year: 20";

char tempMeasuredName[MENU_COLUMNS] = "Temp: ";
char tempWeekdayName[MENU_COLUMNS] = "Weekday: ";
char tempHourName[MENU_COLUMNS] = "Hour: ";
char tempDayName[MENU_COLUMNS] = "Day: ";
char tempNightName[MENU_COLUMNS] = "Night: ";
char tempStateName[MENU_COLUMNS] = "Is:  ";

char heatingIndicator[2][3] = {{' ',25,'\0'},{' ',24,'\0'}};
char *stateIndicator[] = {" Night ", " Day     "};

void menuUpdateTime();
void menuUpdateDate();
void menuUpdateTemp();

void menufuncYear();
void menufuncDay();
void menufuncWeekDay();
void menufuncMonth();

void menufuncHour();
void menufuncMinute();

void menufuncTemp();
void menuUpdateTempMenu();
void menufuncTempDay();
void menufuncTempHour();
void menufuncDayTemp();
void menufuncNightTemp();
void menufuncTempDayNight();


void appendNumber(char *, uint8_t, uint8_t position);
void appendText(char *, const char *, uint8_t position);

struct _menuitem menuA0;
struct _menuitem menuB0;
struct _menuitem menuC0;

struct _menuitem menuC5 = {tempWeekdayName, menufuncTempDay, &menuC0, 0, 0};
struct _menuitem menuC4 = {tempHourName, menufuncTempHour, &menuC0, 0, &menuC5};
struct _menuitem menuC3 = {tempStateName, menufuncTempDayNight, &menuC0, 0, &menuC4};
struct _menuitem menuC2 = {tempNightName, menufuncNightTemp, &menuC0, 0, &menuC3};
struct _menuitem menuC1 = {tempDayName, menufuncDayTemp, &menuC0, 0, &menuC2};
struct _menuitem menuC0 = {"<-", Menu_back, &menu, 0, &menuC1};

struct _menuitem menuB4 = {yearName, menufuncYear, &menuB0, 0, 0};
struct _menuitem menuB3 = {monthName, menufuncMonth, &menuB0, 0, &menuB4};
struct _menuitem menuB2 = {dayName, menufuncDay, &menuB0, 0, &menuB3};
struct _menuitem menuB1 = {weekdayName, menufuncWeekDay, &menuB0, 0, &menuB2};
struct _menuitem menuB0 = {"<-", Menu_back, &menu, 0, &menuB1};

struct _menuitem menuA2 = {minuteName, menufuncMinute, &menuA0, 0, 0};
struct _menuitem menuA1 = {hourName, menufuncHour, &menuA0, 0, &menuA2};
struct _menuitem menuA0 = {"<-", Menu_back, &menu, 0, &menuA1};

struct _menuitem menu2 = {tempMeasuredName, menufuncTemp, &menu, &menuC0, 0};
struct _menuitem menu1 = {dateName, 0, &menu, &menuB0, &menu2};
struct _menuitem menu = {timeName, 0, 0, &menuA0, &menu1};

void Menu_updateNames() { //called with Menu_Show, declared in menu.h
  menuUpdateDate();
  menuUpdateTime();
  menuUpdateTemp();
}

void menufuncYear() {
  ds3231_IncrementDateElement(YearReg, &date.Year);
}

void menufuncMonth() {
  ds3231_IncrementDateElement(MonthReg, &date.Month);
}

void menufuncDay() {
  ds3231_IncrementDateElement(DayReg, &date.Day);
}

void menufuncWeekDay() {
  ds3231_IncrementDateElement(WeekDayReg, &date.WeekDay);
}

void menufuncHour() {
  ds3231_IncrementTimeElement(HoursReg, &time.Hour);
}

void menufuncMinute() {
  ds3231_IncrementTimeElement(MinutesReg, &time.Minute);
}

void menufuncTemp() {
  temp_ResetIndexes();
  menuUpdateTempMenu();
}

void menufuncDayTemp(){
  temp_IncrementDayTemp();
  menuUpdateTempMenu();
}

void menufuncNightTemp(){
  temp_IncrementNightTemp();
  menuUpdateTempMenu();
}

void menufuncTempDayNight(){
  temp_ToggleDayNight();
  menuUpdateTempMenu();
}

void menufuncTempDay() {
  temp_IncrementDayIndex();
  menuUpdateTempMenu();
}

void menufuncTempHour() {
  temp_IncrementHourIndex();
  menuUpdateTempMenu();
}

void menuUpdateTime() {
  ds3231_GetTime(&time);
  ds3231_PrintTime(&time, timeName);
  appendNumber(hourName, time.Hour.value, POS_HOUR);
  appendNumber(minuteName, time.Minute.value, POS_MINUTE);
}

void menuUpdateDate() {
  ds3231_GetDate(&date);
  ds3231_PrintDate(&date, dateName);
  appendText(weekdayName, ds3221_GetDayName(date.WeekDay.value), POS_WEEKDAY);
  appendNumber(dayName, date.Day.value, POS_DAY);
  appendNumber(monthName, date.Month.value, POS_MONTH);
  appendNumber(yearName, date.Year.value, POS_YEAR);
}

void menuUpdateTemp() { 
  temp_PrintTemp(tempMeasuredName, temp_GetMeasuredTemp(),0);
  appendText(tempMeasuredName, stateIndicator[temp_GetSetDayNight()], 6);
  appendText(tempMeasuredName, heatingIndicator[temp.state], 12);
}

void menuUpdateTempMenu() { 
 appendText(tempStateName, stateIndicator[temp_GetSelectedDayNight()], POS_DAYNIGHT);
 appendNumber(tempHourName, temp.hourIndex, POS_HOUR);
 appendText(tempWeekdayName, ds3221_GetDayName(temp.dayIndex), POS_WEEKDAY);
 temp_PrintTemp(tempDayName, temp_GetDayTemp(), POS_TEMP_DAY);
 temp_PrintTemp(tempNightName, temp_GetNightTemp(), POS_TEMP_NIGHT);
}

void appendNumber(char *name, uint8_t number, uint8_t position) {
  char name2[2];
  sprintf(name2, "%02u", number);
  strncpy(name + position, name2, 2);
}

void appendText(char *name1, const char *name2, uint8_t position) {
  strncpy(name1 + position, name2, 10);
}

void Menu_init(){
  ds3231_InitializeTime(&time);
  ds3231_InitializeDate(&date); 
}