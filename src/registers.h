#pragma once
#include<Arduino.h>


#define data_pin 13
#define clock_pin 14
#define latch_pin 12

class REGISTERS{
public:
    REGISTERS();
    uint32_t time_last_update = 0;
    bool state = 0;

    
    void init();
    void loop();
};