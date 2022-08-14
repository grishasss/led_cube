#include "effects.h"


EFFECTS::EFFECTS(){
    
}



void swap(uint8_t &a , uint8_t &b){
    uint8_t tmp = a;
    a = b;
    b = tmp;
}
void EFFECTS::loop(){
    switch (effect_number){
    case 0:
        random_fill();
        break;
    case 1:
        transfusion();
        break;
    default:
        break;
    }

}

void EFFECTS::set_all(uint8_t r , uint8_t g ,uint8_t b){
    for(uint8_t x = 0 ; x < 4 ; x++){
        for(uint8_t y = 0;y < 4; y++){
            for(uint8_t z = 0; z < 4;z++){
                registers->LED[x][y][z][0] = r;
                registers->LED[x][y][z][1] = g;
                registers->LED[x][y][z][2] = b;
            }
        }
    }
}

void EFFECTS::random_fill(){
    if(millis() - time_last_update < 60) return;
    time_last_update = millis();
    static uint8_t iter;
    static uint8_t p[64];
    if(iter == 0){
        set_all(0 , 0 , 0);
        for(uint8_t i = 0; i < 64; i++) p[i] = i;
        for(uint8_t i = 0; i < 64; i++){
            uint8_t a = random(64);
            uint8_t b = random(64);
            swap(p[a]  ,p[b]);
        }
    }
    // registers->LED[p[iter] & 3][(p[iter] >> 2) & 3][(p[iter] >> 4) & 3][0] = 255;
    registers->LED[p[iter] & 3][(p[iter] >> 2) & 3][(p[iter] >> 4) & 3][1] = 255;
    (iter+=1)%=64;
}


void EFFECTS::transfusion(){
    if(millis() - time_last_update < 150) return;
    time_last_update = millis();

    static uint8_t iter;

    static uint8_t R , G , B;
    if(!iter) B = 7;
    switch ( iter / 8){
    case 0:
        G++;
        break;
    case 1:
        B--;
        break;
    case 2:
        R++;
        break;
    case 3:
        G--;
        break;
    case 4:
        B++;
        break;
    case 5:
        R--;
        break;
    default:
        break;
    }
    set_all(R , G , B);


    (iter+=1) %= 48;
}