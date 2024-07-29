#ifndef LIB_OLED_H
#define LIB_OLED_H

#include <Arduino.h>
#include <string.h>

#include "constants.h"
#include "utilsfunc.h"

using uchar = uint8_t;

class OLED {
    public:
        OLED(byte address);
        ~OLED(void);
    
        void init   (void);
        void clear  (void);
        void home   (void);
        void update (void);

        void set_display            (bool flag);
        void set_invert_display     (bool flag);

        void set_horizontal_line    (uchar x0, uchar y0, uchar x1);
        void set_vertical_line      (uchar x0, uchar y0, uchar y1);
        void set_rect               (uchar x0, uchar y0, uchar x1, uchar y1, bool filling = false);
        void set_circle             (uchar x0, uchar y0, uchar radius, bool filling = false);
        void set_cursor             (uchar x, uchar y);
        void set_pixel              (uchar x, uchar y);

        bool get_pixel              (uchar x, uchar y);
        
        void send_command   (byte command);
        void send_data      (byte data);  

    private:
        void __writter(byte, byte);
        void __allocate_memory(void);
        void __free_memory(void);

        byte  __address;
        byte  __font;
        uchar __pos_x;
        uchar __pos_y;
        uchar __buffer[DISPLAY_CONFIG::BUFFER_SIZE];
};


//2474 байт (120%) 
#endif /*LIB_OLED_H*/