#include"registers.h"


REGISTERS::REGISTERS(){

}


void REGISTERS::init(){
    pinMode(clock_pin , OUTPUT);
    pinMode(data_pin , OUTPUT);
    pinMode(latch_pin , OUTPUT);
    pinMode(2 , OUTPUT);
    digitalWrite(latch_pin , 1);
    Serial.println("init led");
}


void REGISTERS::loop(){
    if(millis() - time_last_update > 1000){
        Serial.println("keke");
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