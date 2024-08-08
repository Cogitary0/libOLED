#include "libOLED.h"

extern "C" const uint8_t glcdfont[];

OLED::OLED(uint8_t address) : __address(address){
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
        DISPLAY_COMMANDS::VALUE,
        DISPLAY_COMMANDS::ADDRESSING_MODE,
        DISPLAY_COMMANDS::HORIZONTAL_MODE,
        DISPLAY_COMMANDS::COLUMN_ADDRESS,
        DISPLAY_COMMANDS::HORIZONTAL_MODE,
        DISPLAY_CONFIG::WIDTH,
        DISPLAY_COMMANDS::PAGE_ADDRESS,
        DISPLAY_COMMANDS::HORIZONTAL_MODE,
        DISPLAY_CONFIG::HEIGHT,
        DISPLAY_COMMANDS::SET_DISPLAY_ON
    };

    for(uint8_t command = 0; command < sizeof(INIT); ++command)
    {
        send_command(INIT[command]);
    }

}





///     BASIC COMMANDS      ///

void OLED::clear(void){
    send_command(DISPLAY_COMMANDS::CLEAR_DISPLAY);
    __fill(0x00); //clear buffer
}


void OLED::set_display(bool flag){
    send_command(flag?DISPLAY_COMMANDS::SET_DISPLAY_ON:DISPLAY_COMMANDS::SET_DISPLAY_OFF);
}


void OLED::set_invert_display(bool flag){
    send_command((flag)?DISPLAY_COMMANDS::SET_INVERT_DISPLAY:DISPLAY_COMMANDS::SET_NORMAL_DISPLAY);
}


void OLED::update(){
    uint8_t i, j;
    for(i = 0; i < 64; ++i){
        WRAPPER_BEGINTRANSMISSION(DISPLAY_CONFIG::ADDRESS);
        WRAPPER_WRITE(DISPLAY_COMMANDS::DATA_MODE);
        for(j = 0; j < 16; ++j){
            WRAPPER_WRITE(__buffer[(i<<4) + j]);
        }
        WRAPPER_ENDTRANSMISSION();
    }
}


bool OLED::get_pixel(uint8_t x, uint8_t y){
    return (__buffer[((y & 0xf8) << 4) + x] == 1 << (y & 7));
}

///     DRAW COMMANDS     ///

void OLED::draw_vertical_line(uint8_t x0, uint8_t y0, uint8_t y1){
    _MINMAX(y0, y1);

    if(x0 > DISPLAY_CONFIG::WIDTH){
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

    if(y0 > DISPLAY_CONFIG::HEIGHT){
        return;
    }    

    if(x0 == x1){
        draw_pixel(x0, y0);
        return;
    }

    x0 = constrain(x0, 0, DISPLAY_CONFIG::WIDTH);
    x1 = constrain(x1, 0, DISPLAY_CONFIG::WIDTH);

    for(uint8_t i = x0; i <= x1; ++i){
        draw_pixel(i, y0);
    }
    return;
}


void OLED::draw_line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){

    const uint8_t dx = abs(x1 - x0);
    const uint8_t dy = abs(y1 - y0);
    

    if(dx==0){
        draw_vertical_line(x0, min(y0, y1), max(y0, y1));
        return;
    }

    if(dy == 0){
        draw_horizontal_line(min(x0, x1), y0, max(x0, x1));
        return;
    }

    const int8_t sx = _SIGN(x0, x1);
    const int8_t sy = _SIGN(y0, y1);

    int8_t error = dx - dy;

    while(1){
        draw_pixel(x0, y0);

        if(x0==x1&&y1==y0)break;

        int8_t error2 = 2 * error;


        if(error2 > -dy){
            error -= dy;
            x0 += sx;
        }
        if(error2 < dx){
            error += dx;
            y0 += sy;
        }
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
    
    int8_t x = 0;
    int8_t y = radius;
    int8_t d = 1 - radius;

    while (x <= y) {
        if (filled) {
            for(int8_t i = -y; i <= y; ++i){
                draw_pixel(x0 - x, y0 + i);
                draw_pixel(x0 + x, y0 + i);
            }
        } else {
            draw_pixel(x0 - x, y0 - y);
            draw_pixel(x0 + x, y0 - y);
            draw_pixel(x0 - x, y0 + y);
            draw_pixel(x0 + x, y0 + y);
            draw_pixel(x0 - y, y0 - x);
            draw_pixel(x0 + y, y0 - x);
            draw_pixel(x0 - y, y0 + x);
            draw_pixel(x0 + y, y0 + x);
        }

        if (d < 0) {
            d += (x << 1) + 3;
        } else {
            d += ((x - y) << 1) + 5;
            y--;
        }
        x++;
    }
    return;
    
}


void OLED::draw_pixel(uint8_t x, uint8_t y){
    if(_LIMIT(x,y))return;
    __buffer[((y & 0xF8) << 4) + x] |= 1 << (y & 7);
}


void OLED::draw_triangle(uint8_t x0, uint8_t y0,
                         uint8_t x1, uint8_t y1,
                         uint8_t x2, uint8_t y2){
    draw_line(x0, y0, x1, y1);
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x0, y0);
}






///     SYSTEM COMMANDS     ///

void OLED::send_command(byte command)
{
    __writter(DISPLAY_COMMANDS::COMMAND_MODE, command);
}


void OLED::send_data(byte data)
{
    __writter(DISPLAY_COMMANDS::DATA_MODE, data);
}


uint8_t* OLED::get_buffer(void){
    return __buffer;
}


void OLED::print_buffer(void){
    _PRINT_BUFFER(__buffer);
}


void OLED::clear_buffer(void){
    __fill(0x00);
}


void OLED::fill_buffer(const uint8_t value){
    __fill(value);
}


///     PRIVATE FUNC    ///

inline void OLED::__fill(const uint8_t value){
    memset(__buffer, value, BUFFER_SIZE);
}

inline void OLED::__writter(uint8_t MODE, uint8_t DATA){
    WRAPPER_BEGINTRANSMISSION(__address);
    WRAPPER_WRITE(MODE);
    WRAPPER_WRITE(DATA);
    WRAPPER_ENDTRANSMISSION();
}

