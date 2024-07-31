#ifndef LIB_OLED_UTILS_FUNC_H
#define LIB_OLED_UTILS_FUNC_H

#include <Wire.h>
#include <Arduino.h>

using uchar = unsigned char;

static inline void WRAPPER_INIT(){
    Wire.begin();
}

static inline void WRAPPER_BEGINTRANSMISSION(uchar _address){
    Wire.beginTransmission(_address);
}

static inline void WRAPPER_WRITE(uchar _data){
    Wire.write(_data);
}

static inline void WRAPPER_ENDTRANSMISSION(){
    Wire.endTransmission();
}

static inline void _SWAP(uchar& x, uchar& y){
    x^=y; y^=x; x^=y;
}

static inline void _MINMAX(uchar& p, uchar& q){
    if (p > q) _SWAP(p, q);
}

static inline bool _LIMIT(const uchar x, const uchar y){
    return (x>=DISPLAY_CONFIG::MAX_X || y>=DISPLAY_CONFIG::MAX_Y);
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