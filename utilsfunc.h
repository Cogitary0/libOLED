#ifndef LIB_OLED_UTILS_FUNC_H
#define LIB_OLED_UTILS_FUNC_H

#include <Wire.h>

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



#endif /*LIB_OLED_UTILS_FUNC_H*/