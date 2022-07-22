#pragma once
#include<Arduino.h>
#include "web.h"
#include "registers.h"


class EFFECTS{
public:
    EFFECTS();
    REGISTERS *registers;

    uint32_t time_last_update;
    uint8_t effect_number = 0;
    void random_fill();
    void set_all(uint8_t r , uint8_t g ,uint8_t b);
    void loop();
};