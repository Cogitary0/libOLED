#include "libOLED.h"

OLED::OLED(byte address) : __address(address){
    memset(__buffer, 0, sizeof(__buffer));
}

OLED::~OLED(void){
    delete[] __buffer;
}


void OLED::init(void){
    WRAPPER_INIT();

    static const uchar INIT[] {
        DISPLAY_COMMANDS::DISPLAY_OFF,
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
        DISPLAY_COMMANDS::DISPLAY_ON
    };

    for(uchar command = 0; command < sizeof(INIT); ++command)
    {
        send_command(INIT[command]);
    }

}


///     BASIC COMMANDS      ///

void OLED::clear(void){
    memset(__buffer, 0, sizeof(__buffer));
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




///     SYSTEM COMMANDS     ///

void OLED::send_command(byte command)
{
    __writter(command, DISPLAY_COMMANDS::COMMAND_MODE);
}


void OLED::send_data(byte data)
{
    __writter(data, DISPLAY_COMMANDS::DATA_MODE);
}



///     PRIVATE FUNC    ///

void OLED::__writter(byte DATA, byte MODE)
{
    WRAPPER_BEGINTRANSMISSION(__address);
    WRAPPER_WRITE(MODE);
    WRAPPER_WRITE(DATA);
    WRAPPER_ENDTRANSMISSION();
}
