#include"registers.h"

REGISTERS *pointerToClass;


REGISTERS::REGISTERS(){
    
}
void IRAM_ATTR  REGISTERS::TimerHandler(){
    static uint8_t layer = 0;
    static uint8_t period = 0;
    

    layer++;
    if(layer >= 4){
        layer = 0;
        period++;
        period &= ((1 << 3) - 1);
    }
    memset(registers_state , 0 , sizeof registers_state);
    registers_state[0] = gnd_pins[layer];
    for(uint8_t x = 0 ; x < 4 ; x++){
        for(uint8_t y = 0;y < 4; y++){
            for(uint8_t color = 0;color < 3;color++){
                if(period < LED[x][y][layer][color]) registers_state[(pins[x][y][color] >> 3) + 1] |= (1 << (pins[x][y][color] & 7));
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
    
    ITimer.attachInterruptInterval(600, [](){
        pointerToClass->TimerHandler();
    });
    Serial.println("inited led");
}



void REGISTERS::loop(){
    

}

void REGISTERS::change_status(bool mode){
    delay(100); // tmp fix
    status = mode;
    if(mode){
        ITimer.enableTimer();
    }
    else{
        ITimer.disableTimer();
    }
}