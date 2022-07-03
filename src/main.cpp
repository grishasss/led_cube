#include "led_cube.h"

LED_CUBE led_cube;

void setup() {
    led_cube.begin();
}

void loop() {
    led_cube.process();
}