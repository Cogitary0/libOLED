#ifndef LIB_OLED_CONSTANTS_H
#define LIB_OLED_CONSTANTS_H


enum DISPLAY_CONFIG : uint8_t{
    WIDTH = 0x7F,
    HEIGHT = 0x3F,
    ADDRESS = 0x3C
};


enum DISPLAY_COMMANDS : uint8_t{
    /// SETUP
    SET_DISPLAY_ON  = 0xAF,
    SET_DISPLAY_OFF = 0xAE,
    SET_NORMAL_DISPLAY = 0xA6,
    SET_INVERT_DISPLAY = 0xA7,
    SET_DISPLAY_CLOCK_DIV = 0xD5,
    SET_MULTIPLEX = 0xA8,
    SET_DISPLAY_OFFSET = 0xD3,
    SET_CONTRAST = 0x81,
    SET_PRECHARGE = 0xD9,
    SET_BRIGHTNESS = 0x81,

    /// MODE
    COMMAND_MODE = 0x80,
    DATA_MODE = 0x40,
    VERTICAL_MODE = 0x01,
    ADDRESSING_MODE = 0x20,
    HORIZONTAL_MODE = 0x00,
    COLUMN_ADDRESS = 0x21,
    PAGE_ADDRESS = 0x22,

    /// COMMANDS
    CHARGE_PUMP = 0x8D,
    CLEAR_DISPLAY = 0x00,
    VALUE = 0x14

};


enum SPI_SETTING {
    SPEED = 1000000ul,
    BITORDER = 1,
    MODE0 = 0x00
};

static constexpr uint16_t BUFFER_SIZE = 1024;  /// MAX_X * MAX_Y / 8
static constexpr uint16_t EEPROM_SIZE = 1024;
static constexpr uint16_t EEPORM_PAGE_SIZE = 4;
static constexpr uint16_t FLASH_SIZE = 30720;
static constexpr uint16_t FLASH_PAGE_SIZE = 64;



#endif /*LIB_OLED_CONSTANTS_H*/