#pragma once

#include <Arduino.h>
#include <EEPROM.h>
#include <FS.h>


class Memory{
public:
    static void init();
    static void write_bit(bool f , uint16_t pos);
    static void write_float(float f, uint16_t pos);
    static void write_uint32(uint32_t f, uint16_t pos);
    static void write_uint16(uint16_t f, uint16_t pos);

    static bool get_bit(uint16_t pos);
    static float get_float(uint16_t pos);
    static uint16_t get_uint16(uint16_t pos);
    static uint32_t get_uint32(uint16_t pos);
};
