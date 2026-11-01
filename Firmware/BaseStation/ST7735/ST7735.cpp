#include "ST7735.h"
#include <cmath>
#include <cstdlib>

extern const uint8_t font5x7[];

ST7735::ST7735(spi_inst_t* spi, uint pin_cs, uint pin_dc, uint pin_rst, uint pin_sck, uint pin_mosi, uint pin_bl)
    : _spi(spi), _cs(pin_cs), _dc(pin_dc), _rst(pin_rst), _sck(pin_sck), _mosi(pin_mosi), _bl(pin_bl) {}

void ST7735::writeCommand(uint8_t cmd) {
    gpio_put(_dc, 0);
    gpio_put(_cs, 0);
    spi_write_blocking(_spi, &cmd, 1);
    gpio_put(_cs, 1);
}

void ST7735::writeData(uint8_t data) {
    gpio_put(_dc, 1);
    gpio_put(_cs, 0);
    spi_write_blocking(_spi, &data, 1);
    gpio_put(_cs, 1);
}

void ST7735::init() {
    spi_init(_spi, 20000000); // 20 MHz
    gpio_set_function(_sck, GPIO_FUNC_SPI);
    gpio_set_function(_mosi, GPIO_FUNC_SPI);

    gpio_init(_cs);
    gpio_set_dir(_cs, GPIO_OUT);
    gpio_put(_cs, 1);

    gpio_init(_dc);
    gpio_set_dir(_dc, GPIO_OUT);
    gpio_put(_dc, 1);

    gpio_init(_rst);
    gpio_set_dir(_rst, GPIO_OUT);

    gpio_init(_bl);
    gpio_set_dir(_bl, GPIO_OUT);
    gpio_put(_bl, 1);

    // Reset sequence
    gpio_put(_rst, 1);
    sleep_ms(10);
    gpio_put(_rst, 0);
    sleep_ms(20);
    gpio_put(_rst, 1);
    sleep_ms(150);

    writeCommand(0x01); // SWRESET
    sleep_ms(150);

    writeCommand(0x11); // SLPOUT
    sleep_ms(255);

    writeCommand(0xB1); // Frame Rate
    writeData(0x01); writeData(0x2C); writeData(0x2D);

    // Power sequence
    writeCommand(0xC0); writeData(0xA2); writeData(0x02); writeData(0x84);
    writeCommand(0xC1); writeData(0xC5);
    writeCommand(0xC2); writeData(0x0A); writeData(0x00);
    writeCommand(0xC3); writeData(0x8A); writeData(0x2A);
    writeCommand(0xC4); writeData(0x8A); writeData(0xEE);
    writeCommand(0xC5); writeData(0x0E);

    writeCommand(0x36); // MADCTL
    writeData(0xC0);    // Portrait mode

    writeCommand(0x3A); // 16-bit RGB565
    writeData(0x05);

    _colstart = 2;
    _rowstart = 1;

    writeCommand(0x29); // DISPON
    sleep_ms(100);
}

void ST7735::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    writeCommand(0x2A);
    writeData(0x00); writeData(x0 + _colstart);
    writeData(0x00); writeData(x1 + _colstart);

    writeCommand(0x2B);
    writeData(0x00); writeData(y0 + _rowstart);
    writeData(0x00); writeData(y1 + _rowstart);

    writeCommand(0x2C);
}

void ST7735::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (x >= WIDTH || y >= HEIGHT || w <= 0 || h <= 0) return;
    if (x + w > WIDTH) w = WIDTH - x;
    if (y + h > HEIGHT) h = HEIGHT - y;

    setAddrWindow(x, y, x + w - 1, y + h - 1);
    gpio_put(_dc, 1);
    gpio_put(_cs, 0);

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;
    uint32_t total = (uint32_t)w * h;

    uint8_t buf[32];
    for (int i = 0; i < 32; i += 2) {
        buf[i] = hi;
        buf[i + 1] = lo;
    }
    while (total > 0) {
        uint32_t count = (total > 16) ? 16 : total;
        spi_write_blocking(_spi, buf, count * 2);
        total -= count;
    }
    gpio_put(_cs, 1);
}

void ST7735::fillScreen(uint16_t color) {
    fillRect(0, 0, WIDTH, HEIGHT, color);
}

void ST7735::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    fillRect(x, y, w, 1, color);
}

void ST7735::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    fillRect(x, y, 1, h, color);
}

void ST7735::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep) { std::swap(x0, y0); std::swap(x1, y1); }
    if (x0 > x1) { std::swap(x0, x1); std::swap(y0, y1); }

    int16_t dx = x1 - x0;
    int16_t dy = std::abs(y1 - y0);
    int16_t err = dx / 2;
    int16_t ystep = (y0 < y1) ? 1 : -1;

    for (; x0 <= x1; x0++) {
        if (steep) fillRect(y0, x0, 1, 1, color);
        else       fillRect(x0, y0, 1, 1, color);
        err -= dy;
        if (err < 0) {
            y0 += ystep;
            err += dx;
        }
    }
}

void ST7735::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    fillRect(x0, y0 + r, 1, 1, color);
    fillRect(x0, y0 - r, 1, 1, color);
    fillRect(x0 + r, y0, 1, 1, color);
    fillRect(x0 - r, y0, 1, 1, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        fillRect(x0 + x, y0 + y, 1, 1, color);
        fillRect(x0 - x, y0 + y, 1, 1, color);
        fillRect(x0 + x, y0 - y, 1, 1, color);
        fillRect(x0 - x, y0 - y, 1, 1, color);
        fillRect(x0 + y, y0 + x, 1, 1, color);
        fillRect(x0 - y, y0 + x, 1, 1, color);
        fillRect(x0 + y, y0 - x, 1, 1, color);
        fillRect(x0 - y, y0 - x, 1, 1, color);
    }
}

void ST7735::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    drawFastVLine(x0, y0 - r, 2 * r + 1, color);
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        drawFastVLine(x0 + x, y0 - y, 2 * y + 1, color);
        drawFastVLine(x0 - x, y0 - y, 2 * y + 1, color);
        drawFastVLine(x0 + y, y0 - x, 2 * x + 1, color);
        drawFastVLine(x0 - y, y0 - x, 2 * x + 1, color);
    }
}

#include "glcdfont.c"

void ST7735::drawChar(int16_t x, int16_t y, char c, uint16_t color, uint16_t bg, uint8_t size) {
    if (c < 32 || c > 126) c = '?';
    for (int8_t i = 0; i < 5; i++) {
        uint8_t line = font5x7[(c - 32) * 5 + i];
        for (int8_t j = 0; j < 8; j++, line >>= 1) {
            if (line & 1) {
                fillRect(x + i * size, y + j * size, size, size, color);
            } else if (bg != color) {
                fillRect(x + i * size, y + j * size, size, size, bg);
            }
        }
    }
}

void ST7735::drawString(int16_t x, int16_t y, const char* str, uint16_t color, uint16_t bg, uint8_t size) {
    while (*str) {
        drawChar(x, y, *str, color, bg, size);
        x += size * 6;
        str++;
    }
}