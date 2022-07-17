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
    

    bool state = 0;

    void change_led_status(uint8_t state,  uint8_t X ,uint8_t Y , uint8_t Z);
    
    
    void init();
    void loop();

    void IRAM_ATTR TimerHandler();
};
