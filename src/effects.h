#pragma once
#include<Arduino.h>
#include "registers.h"

#define effect_count 1

class EFFECTS{
public:
    EFFECTS();
    REGISTERS *registers;
    

    String effect_name[effect_count] = {
        "random fill"
    };

    uint32_t time_last_update;
    uint8_t effect_number = 0;
    void random_fill();
    void set_all(uint8_t r , uint8_t g ,uint8_t b);
    void loop();
};