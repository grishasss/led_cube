#include"registers.h"

REGISTERS *pointerToClass;


REGISTERS::REGISTERS(){
    
}
void IRAM_ATTR  REGISTERS::TimerHandler(){
    static uint8_t layer = 0;
    static uint8_t period = 0;
    

    layer++;
    if(layer == 4){
        layer = 0;
        period++;
        period &= ((1 << 6) - 1);
    }
    memset(registers_state , 0 , sizeof registers_state);
    switch(layer){
    case 0:
        registers_state[0] = 8;
        break;
    case 1:
        registers_state[0] = 2;
        break;
    case 2:
        registers_state[0] = 4;
        break;
    case 3:
        registers_state[0] = 16;
        break;
    }
    for(uint8_t x = 0 ; x < 4 ; x++){
        for(uint8_t y = 0;y < 4; y++){
            for(uint8_t color = 0;color < 3;color++){
                if(period < LED[x][y][layer][color]) registers_state[pins[x][y][color] >> 3] |= (1 <<  (pins[x][y][color] & 7));
            }
        }
    }
    digitalWrite(latch_pin , 0);
    for(int8_t i = 6 ; i>=0; i--){
        shiftOut(data_pin, clock_pin, MSBFIRST, registers_state[i]);
    }
    digitalWrite(latch_pin , 1);
}



void REGISTERS::init(){
    pointerToClass = this;
    pinMode(clock_pin , OUTPUT);
    pinMode(data_pin , OUTPUT);
    pinMode(latch_pin , OUTPUT);
    pinMode(2 , OUTPUT);
    digitalWrite(latch_pin , 1);
    
    ITimer.attachInterruptInterval(550, [](){
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