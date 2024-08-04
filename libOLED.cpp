#include "libOLED.h"

OLED::OLED(byte address) : __address(address){
    memset(__buffer, 0, sizeof(__buffer));
}

OLED::~OLED(void){
    delete[] __buffer;
}


void OLED::init(void){
    WRAPPER_INIT();

    static const uint8_t INIT[] {
        DISPLAY_COMMANDS::SET_DISPLAY_OFF,
        DISPLAY_COMMANDS::CHARGE_PUMP,
        0x14,
        DISPLAY_COMMANDS::ADDRESSING_MODE,
        DISPLAY_COMMANDS::HORIZONTAL_ADDRESS,
        DISPLAY_COMMANDS::COLUMN_ADDRESS,
        DISPLAY_COMMANDS::HORIZONTAL_ADDRESS,
        DISPLAY_CONFIG::MAX_X,
        DISPLAY_COMMANDS::PAGE_ADDRESS,
        DISPLAY_COMMANDS::HORIZONTAL_ADDRESS,
        DISPLAY_CONFIG::MAX_Y,
        DISPLAY_COMMANDS::SET_DISPLAY_ON
    };

    for(uint8_t command = 0; command < sizeof(INIT); ++command)
    {
        send_command(INIT[command]);
    }

}


///     BASIC COMMANDS      ///

void OLED::clear(void){
    memset(__buffer, NULL, sizeof(__buffer));
}


void OLED::set_display(bool flag){
    send_command(flag?DISPLAY_COMMANDS::SET_DISPLAY_ON:DISPLAY_COMMANDS::SET_DISPLAY_OFF);
}


void OLED::set_invert_display(bool flag){
    send_command((flag)?DISPLAY_COMMANDS::SET_INVERT_DISPLAY:DISPLAY_COMMANDS::SET_NORMAL_DISPLAY);
}


void OLED::update(){
    for(int i = 0; i < 64; ++i){
        WRAPPER_BEGINTRANSMISSION(DISPLAY_CONFIG::ADDRESS);
        WRAPPER_WRITE(0x40);
        for(int j = 0; j < 16; ++j){
            WRAPPER_WRITE(__buffer[(i<<4) + j]);
        }
        WRAPPER_ENDTRANSMISSION();
    }
}


bool OLED::get_pixel(uint8_t x, uint8_t y){
    return (__buffer[((y&0xf8)<<4)+x] == 1 << (y & 7));
}


///     DRAW COMMANDS     ///


void OLED::draw_vertical_line(uint8_t x0, uint8_t y0, uint8_t y1){
    _MINMAX(y0, y1);

    if(x0 > DISPLAY_CONFIG::MAX_X){
        return;
    }

    if(y0 == y1){
        draw_pixel(x0, y0);
        return;
    }

    for(uint8_t i = y0; i <= y1; ++i){
        draw_pixel(x0, i);
    }
    return;
}


void OLED::draw_horizontal_line(uint8_t x0, uint8_t y0, uint8_t x1){
    _MINMAX(x0, x1);

    if(y0 > DISPLAY_CONFIG::MAX_Y){
        return;
    }    

    if(x0 == x1){
        draw_pixel(x0, y0);
        return;
    }

    x0 = constrain(x0, 0, DISPLAY_CONFIG::MAX_X);
    x1 = constrain(x1, 0, DISPLAY_CONFIG::MAX_X);

    for(uint8_t i = x0; i <= x1; ++i){
        draw_pixel(i, y0);
    }
    return;
}


void OLED::draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){

    _MINMAX(x0, x1);
    _MINMAX(y0, y1);

    uint8_t diff = abs(y1 - y0)/abs(x1 - x0);

    for(uint8_t x = x0; x <= x1; ++x){
        draw_pixel(x, (uint8_t)(y0 + ((x-x0)*diff)));
    }

    return;

}


void OLED::draw_rect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool filled){
    _MINMAX(x0, x1);
    _MINMAX(y0, y1);

    if(x0 == x1) {
        draw_vertical_line(x0, y0, y1);
        return;
    }

    if(y0 == y1) {
        draw_horizontal_line(x0, y0, x1);
        return;
    }

    if(!filled){
        draw_vertical_line      (x1, y0, y1);
        draw_horizontal_line    (x0, y0, x1);
        draw_vertical_line      (x0, y0, y1);
        draw_horizontal_line    (x0, y1, x1);
        return;
    }

    else {
        for(uint8_t i = x0; i <= x1; ++i){
            for(uint8_t j = y0; j <= y1; ++j){
                draw_pixel(i, j);
            }
        }
    }
    return;
}


void OLED::draw_circle(uint8_t x0, uint8_t y0, uint8_t radius, bool filled){
    if(radius == 0) return;
    
    if(radius == 1) {
        draw_pixel(x0, y0); 
        return;
    }

    
}


void OLED::draw_pixel(uint8_t x, uint8_t y){
    if(_LIMIT(x,y))return;
    __buffer[((y&0xF8)<<4)+x] |= 1 << (y&7);
}


void OLED::draw_triangle(uint8_t x0, uint8_t y0,
                         uint8_t x1, uint8_t y1,
                         uint8_t x2, uint8_t y2){
    draw_line(x0, y0, x1, y1);
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x0, y0);
}


void OLED::draw_char(uint8_t x, uint8_t y, const char ch){
    
}


///     SYSTEM COMMANDS     ///

void OLED::send_command(byte command)
{
    __writter(command, DISPLAY_COMMANDS::COMMAND_MODE);
}


void OLED::send_data(byte data)
{
    __writter(data, DISPLAY_COMMANDS::DATA_MODE);
}


uint8_t* OLED::get_buffer(void){
    return __buffer;
}


void OLED::print_buffer(void){
    _PRINT_BUFFER(__buffer);
}

///     PRIVATE FUNC    ///

void OLED::__writter(byte DATA, byte MODE)
{
    WRAPPER_BEGINTRANSMISSION(__address);
    WRAPPER_WRITE(MODE);
    WRAPPER_WRITE(DATA);
    WRAPPER_ENDTRANSMISSION();
}


