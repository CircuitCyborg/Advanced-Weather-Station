#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "ST7735.h"
#include "WeatherScreen.h"
#include "WeatherData.h"
#include "InputManager.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

// Display SPI Configuration (spi0)
#define TFT_SPI     spi0
#define TFT_SCK     6
#define TFT_MOSI    7
#define TFT_MISO    4
#define TFT_CS      5
#define TFT_DC      20
#define TFT_RST     26
#define TFT_BL      19

// Weather Station RJ45 Link (uart0 on GPIO 0 & 1)
#define SENSOR_UART uart0
#define SENSOR_TX   0
#define SENSOR_RX   1
#define SENSOR_BAUD 115200

// XIAO ESP32-C5 UART Link (uart1 on GPIO 21 & 22)
#define ESP32_UART  uart1
#define ESP32_TX    21
#define ESP32_RX    22
#define ESP32_BAUD  115200

// Parses incoming line: "Temp=75.200000,HUM=45.000000,DIR=315.000000\n"
void parseWeatherPacket(const char* line, WeatherData& data) {
    const char* pTemp = strstr(line, "Temp=");
    if (pTemp) {
        data.temp_f = strtof(pTemp + 5, nullptr);
    }
    const char* pHum = strstr(line, "HUM=");
    if (pHum) {
        data.humidity_pct = strtof(pHum + 4, nullptr);
    }
    const char* pDir = strstr(line, "DIR=");
    if (pDir) {
        data.wind_dir_deg = strtof(pDir + 4, nullptr);
    }
    const char* pRain = strstr(line, "RAIN=");
    if (pRain) {
        data.precip_rate_in = strtof(pRain + 5, nullptr);
    }
}

int main() {
    stdio_init_all();

    // 1. Initialize Outdoor Station UART Link (uart0)
    uart_init(SENSOR_UART, SENSOR_BAUD);
    gpio_set_function(SENSOR_TX, GPIO_FUNC_UART);
    gpio_set_function(SENSOR_RX, GPIO_FUNC_UART);

    // 2. Initialize ESP32-C5 UART Link (uart1)
    uart_init(ESP32_UART, ESP32_BAUD);
    gpio_set_function(ESP32_TX, GPIO_FUNC_UART);
    gpio_set_function(ESP32_RX, GPIO_FUNC_UART);

    // 3. Inputs & Display
    InputManager inputs;
    inputs.init();

    ST7735 tft(TFT_SPI, TFT_CS, TFT_DC, TFT_RST, TFT_SCK, TFT_MOSI, TFT_BL);
    tft.init();

    WeatherScreen screen(tft);
    screen.init();

    WeatherData currentData;

    char rxBuffer[128];
    size_t rxIndex = 0;

    while (true) {
        // Read incoming sensor data from the outdoor station Pico
        while (uart_is_readable(SENSOR_UART)) {
            char c = uart_getc(SENSOR_UART);
            if (c == '\n' || c == '\r') {
                if (rxIndex > 0) {
                    rxBuffer[rxIndex] = '\0';
                    parseWeatherPacket(rxBuffer, currentData);
                    rxIndex = 0;
                }
            } else if (rxIndex < sizeof(rxBuffer) - 1) {
                rxBuffer[rxIndex++] = c;
            }
        }

        // Poll button inputs
        uint8_t btns = inputs.readButtons();
        (void)btns;

        // Refresh UI screen
        screen.update(currentData);
        sleep_ms(100);
    }

    return 0;
}