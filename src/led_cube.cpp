#include "led_cube.h"
#include "web.h"


LED_CUBE::LED_CUBE(){

}


WEB web;

void LED_CUBE::begin(){
    Serial.begin(115200);
    memory_init();
    
    web.wifi_init();
    web.start_all_server();
}


void LED_CUBE::process(){
    web.loop();
}