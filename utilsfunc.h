#ifndef LIB_OLED_UTILS_FUNC_H
#define LIB_OLED_UTILS_FUNC_H

#include <Wire.h>

inline void WRAPPER_INIT(){
    Wire.begin();
}

inline void WRAPPER_BEGINTRANSMISSION(uint8_t _address){
    Wire.beginTransmission(_address);
}

inline void WRAPPER_WRITE(uint8_t _data){
    Wire.write(_data);
}

inline void WRAPPER_ENDTRANSMISSION(){
    Wire.endTransmission();
}

inline void _SWAP(uint8_t& x, uint8_t& y){
    x^=y; y^=x; x^=y;
}

inline void _MINMAX(uint8_t& p, uint8_t& q){
    if (p > q) _SWAP(p, q);
}

inline bool _LIMIT(const uint8_t x, const uint8_t y){
    return (x>=DISPLAY_CONFIG::WIDTH || y>=DISPLAY_CONFIG::HEIGHT);
}

inline int8_t _SIGN(const uint8_t value){
    return value < 0 ? -1 : 1;
}

inline int8_t _SIGN(const uint8_t x, const uint8_t y){
    return x < y ? 1 : -1;
}

inline void _PRINT_BUFFER(const void* buffer){
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