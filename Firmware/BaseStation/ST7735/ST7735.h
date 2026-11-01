#pragma once
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define TFT_BLACK       0x0000
#define TFT_NAVY        0x000F
#define TFT_DARKGREY    0x39E7
#define TFT_LIGHTGREY   0xC618
#define TFT_WHITE       0xFFFF
#define TFT_RED         0xF800
#define TFT_GREEN       0x07E0
#define TFT_CYAN        0x07FF
#define TFT_YELLOW      0xFFE0
#define TFT_BG          0x0841

class ST7735 {
public:
    static const int WIDTH  = 128;
    static const int HEIGHT = 160;

    ST7735(spi_inst_t* spi, uint pin_cs, uint pin_dc, uint pin_rst, uint pin_sck, uint pin_mosi, uint pin_bl);

    void init();
    void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void fillScreen(uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);

    void drawChar(int16_t x, int16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
    void drawString(int16_t x, int16_t y, const char* str, uint16_t color, uint16_t bg, uint8_t size);

private:
    spi_inst_t* _spi;
    uint _cs, _dc, _rst, _sck, _mosi, _bl;
    uint8_t _colstart = 2;
    uint8_t _rowstart = 1;

    void writeCommand(uint8_t cmd);
    void writeData(uint8_t data);
};