#include "memory.h"


void memory_init(){
    EEPROM.begin(512);
    SPIFFS.begin();
}

void memory_write_bit(bool f , uint16_t pos){
    uint8_t ls = EEPROM.read(pos >> 3);
    uint8_t cnt_bit = 7 - pos % 8;
    ls ^= (ls & (1 << cnt_bit));
    ls |= ((uint8_t)f << cnt_bit); 
    EEPROM.write(pos >> 3 , ls); 
    EEPROM.commit();
}

bool memory_get_bit(uint16_t pos){
    uint8_t b = EEPROM.read(pos >> 3);
    uint8_t cnt_bit = 7 - pos % 8;
    return (bool)(b & (1 << cnt_bit));
}

void memory_write_uint32(uint32_t f, uint16_t pos){
    EEPROM.write(f & ((1 << 8) - 1) , pos + 3);
    f>>=8;
    EEPROM.write(f & ((1 << 8) - 1) , pos + 2);
    f>>=8;
    EEPROM.write(f & ((1 << 8) - 1) , pos + 1);
    f>>=8;
    EEPROM.write(f & ((1 << 8) - 1) , pos );
    EEPROM.commit();
}

void memory_write_uint16(uint16_t f, uint16_t pos){
    
    EEPROM.write(f & ((1 << 8) - 1) , pos + 1);
    f>>=8;
    EEPROM.write(f & ((1 << 8) - 1) , pos);
    EEPROM.commit();
}


void memory_write_float(float f, uint16_t pos){
    uint32_t tmp;
    memcpy(&tmp, &f , sizeof f);
    memory_write_uint32(tmp ,pos);
}


float memory_get_float(uint16_t pos){
    uint32_t tmp = memory_get_uint32(pos);
    float f;
    memcpy(&f  , &tmp , sizeof tmp);
    return f;
}


uint16_t memory_get_uint16(uint16_t pos){
    uint16_t f = EEPROM.read(pos + 1);
    f |= (EEPROM.read(pos) << 8);
    return f;
}


uint32_t memory_get_uint32(uint16_t pos){
    uint32_t f = EEPROM.read(pos + 3);
    f |= (EEPROM.read(pos + 2) << 8);
    f |= (EEPROM.read(pos + 1) << 16);
    f |= (EEPROM.read(pos) << 24);
    return f;
}

