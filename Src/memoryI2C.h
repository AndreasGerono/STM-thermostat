
//  
//  External I2C eeprom library
//


#include "stm32l0xx_hal.h"

#define TIMEOUT 100
#define MEM_ADDR  0xAe  //memory addres shifted to left
#define MEM_I2C &hi2c3 //define used I2c
#define SIZE 1 
#define  DAYNIGHT_SECTION 0
#define  TEMP_SECTION 200


void memory_WriteByte(uint16_t, uint8_t);
uint16_t memory_GetAddress(uint8_t day,uint8_t, uint16_t section);
uint8_t memory_ReadByte(uint16_t);
uint8_t memory_ReadByte(uint16_t);