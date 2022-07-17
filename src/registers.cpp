#include"registers.h"

REGISTERS *pointerToClass;


REGISTERS::REGISTERS(){
    
}
void IRAM_ATTR  REGISTERS::TimerHandler(){
    
}



void REGISTERS::init(){
    pointerToClass = this;
    pinMode(clock_pin , OUTPUT);
    pinMode(data_pin , OUTPUT);
    pinMode(latch_pin , OUTPUT);
    pinMode(2 , OUTPUT);
    digitalWrite(latch_pin , 1);
    
    ITimer.attachInterruptInterval(100000, [](){
        pointerToClass->TimerHandler();
    });
    Serial.println("init led");
}


void REGISTERS::change_led_status(uint8_t state,  uint8_t X ,uint8_t Y , uint8_t Z){
    pointerToClass->TimerHandler(); 
}

void REGISTERS::loop(){
    if(millis() - time_last_update > 1000){
        time_last_update = millis();
        state = !state;
        uint8_t val = 0;
        if(state) val = 255;
        digitalWrite(2 , state);
        digitalWrite(latch_pin , 0);
        shiftOut(data_pin, clock_pin, MSBFIRST, val);
        shiftOut(data_pin, clock_pin, MSBFIRST, val);
        shiftOut(data_pin, clock_pin, MSBFIRST, val);
        shiftOut(data_pin, clock_pin, MSBFIRST, ~val);
        shiftOut(data_pin, clock_pin, MSBFIRST, val);
        digitalWrite(latch_pin , 1);
    }
}