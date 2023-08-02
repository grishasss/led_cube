#include "button.h"

BUTTON::BUTTON(uint8 _pin){
    pin = _pin;
    pinMode(pin , INPUT_PULLUP);
}

void BUTTON::loop(){
    
}