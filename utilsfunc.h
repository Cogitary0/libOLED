#ifndef LIB_OLED_UTILS_FUNC_H
#define LIB_OLED_UTILS_FUNC_H

#include <Wire.h>
#include <Arduino.h>

static inline void WRAPPER_INIT(){
    Wire.begin();
}

static inline void WRAPPER_BEGINTRANSMISSION(uint8_t _address){
    Wire.beginTransmission(_address);
}

static inline void WRAPPER_WRITE(uint8_t _data){
    Wire.write(_data);
}

static inline void WRAPPER_ENDTRANSMISSION(){
    Wire.endTransmission();
}

static inline void _SWAP(uint8_t& x, uint8_t& y){
    x^=y; y^=x; x^=y;
}

static inline void _MINMAX(uint8_t& p, uint8_t& q){
    if (p > q) _SWAP(p, q);
}

static inline bool _LIMIT(const uint8_t x, const uint8_t y){
    return (x>=DISPLAY_CONFIG::MAX_X || y>=DISPLAY_CONFIG::MAX_Y);
}

static inline int8_t _SIGN(const uint8_t value){
    return (value < 0) ? -1 : 1;
}

static inline void _PRINT_BUFFER(const void* buffer){
    constexpr uint8_t SIZE_PRINT = 64;
    char str[5];

    sprintf_P(str, PSTR("%04X"), (uint16_t)(buffer));
    Serial.print(str); // address

    buffer -= SIZE_PRINT;

    for(uint8_t i = 0; i < SIZE_PRINT; ++i){
        sprintf_P(str, PSTR(" %02X"), *(uint8_t*)buffer);
        Serial.print(str);
        buffer++;
    }
    Serial.print('\n');
}



#endif /*LIB_OLED_UTILS_FUNC_H*/