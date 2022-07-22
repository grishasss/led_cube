#include "led_cube.h"
#include "web.h"
#include "registers.h"
#include "effects.h"


LED_CUBE::LED_CUBE(){

}

WEB web;
REGISTERS registers;
EFFECTS effects;

void LED_CUBE::begin(){
    Serial.begin(115200);

    web.registors = (&registers);
    effects.registers = (&registers);
    memory_init();
    web.wifi_init();
    web.start_all_server();

    registers.init();
}


void LED_CUBE::process(){
    web.loop();
    registers.loop();
    effects.loop();
}