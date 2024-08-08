#ifndef LIB_OLED_H
#define LIB_OLED_H

#include <Arduino.h>
#include <string.h>
#include <avr/pgmspace.h>
#include <SPI.h>

#include "constants.h"
#include "utilsfunc.h"

static SPISettings OLED_SPI_SET(8000000ul,
                                SPI_SETTING::BITORDER,
                                SPI_SETTING::MODE0);

class OLED{
    inline void __writter(uint8_t, uint8_t);
    inline void __fill(uint8_t);

    const uint8_t  __address;
    uint8_t __buffer[BUFFER_SIZE];

    public:
        OLED(uint8_t address);
        ~OLED(void);
    
        void init   (void);
        void clear  (void);
        void update (void);

        void set_display             (bool flag);
        void set_invert_display      (bool flag);

        void draw_line               (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
        void draw_horizontal_line    (uint8_t x0, uint8_t y0, uint8_t x1);
        void draw_vertical_line      (uint8_t x0, uint8_t y0, uint8_t y1);
        void draw_rect               (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const bool filled = true);
        void draw_circle             (uint8_t x0, uint8_t y0, uint8_t radius, const bool filled = false);
        void draw_triangle           (uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
        void draw_char               (uint8_t x,  uint8_t y, const char ch);
        void draw_pixel              (uint8_t x,  uint8_t y);

        void clear_pixel             (uint8_t x, uint8_t y);
        bool get_pixel               (uint8_t x, uint8_t y);
        
        void send_command   (uint8_t command);
        void send_data      (uint8_t data);  

        uint8_t* get_buffer   (void);
        void     print_buffer (void);       
        void     clear_buffer (void);
        void     fill_buffer  (const uint8_t value);
        
};



#endif /*LIB_OLED_H*/