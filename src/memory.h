#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include <FS.h>


    
void memory_init();
void memory_write_bit(bool f , uint16_t pos);
void memory_write_float(float f, uint16_t pos);
void memory_write_uint32(uint32_t f, uint16_t pos);
void memory_write_uint16(uint16_t f, uint16_t pos);

bool memory_get_bit(uint16_t pos);
float memory_get_float(uint16_t pos);
uint16_t memory_get_uint16(uint16_t pos);
uint32_t memory_get_uint32(uint16_t pos);
