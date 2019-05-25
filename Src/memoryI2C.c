#include "memoryI2C.h"
#include "i2c.h"
#include "stm32l0xx_hal.h"

uint16_t memory_GetAddress(uint8_t day, uint8_t hour, uint16_t section) {
  uint16_t addr = section;
  for (uint8_t i = 1; i < day; i++)
      addr+=24;
  for (uint8_t i = 0; i < hour; i++)
    addr++;
  return  addr;
}

void memory_WriteByte(uint16_t addr, uint8_t data) {
  HAL_I2C_Mem_Write(MEM_I2C, MEM_ADDR, addr, I2C_MEMADD_SIZE_16BIT, &data, SIZE, TIMEOUT);
  while (HAL_I2C_IsDeviceReady(MEM_I2C, MEM_ADDR, 1, TIMEOUT))
    ;
}

uint8_t memory_ReadByte(uint16_t addr) {
  uint8_t data;
  HAL_I2C_Mem_Read(MEM_I2C, MEM_ADDR, addr, I2C_MEMADD_SIZE_16BIT, &data, SIZE, TIMEOUT);
  return data;
}