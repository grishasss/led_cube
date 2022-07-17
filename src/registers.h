#pragma once
#include<Arduino.h>
#include <ESP8266TimerInterrupt.h>


#define data_pin 13
#define clock_pin 14
#define latch_pin 12

class REGISTERS{

public:
    REGISTERS();
    ESP8266Timer ITimer;
    
    uint32_t time_last_update = 0;
    
    uint8_t gnd_pin_status = 0;
    uint8_t vcc_pin_status[6];
    
    volatile uint8_t LED[4][4][4][3];
    const uint8_t pins[4][4][3] = 
    {
        {{4*8+2 , 4*8+1 , 5*8+7}, {5*8+6, 5*8+5, 5*8+4}, {5*8+3 , 5*8+2, 5*8 + 1} , {0*8+0 , 4*8+0 , 5*8+0}},
        {{3*8+7, 3*8+6, 3*8+5} , {3*8+4, 3*8+3, 3*8+2} , {3*8+1, 4*8+7, 4*8+6} , {4*8+5, 4*8+4 , 4*8+3}},
        {{1*8+2, 1*8+1 , 2*8+7} , {2*8+6 , 2*8+5, 2*8+4} , {2*8+3, 2*8+2, 2*8+1} , {1*8+0 , 2*8+0 , 3*8+0}},
        {{0*8+7 , 0*8+6, 0*8+5} , {0*8+4 , 0*8+3, 0*8+2} , {0*8+1, 1*8+7, 1*8+6} , {1*8+5 , 1*8+4, 1*8+3}}
    }; 
    uint8_t registers_state[7];

    bool state = 0;

    void change_led_status(uint8_t state,  uint8_t X ,uint8_t Y , uint8_t Z);
    
    
    void init();
    void loop();

    void IRAM_ATTR TimerHandler();
};
