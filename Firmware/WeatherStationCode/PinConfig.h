#pragma once

#include "pico/stdlib.h"

struct PinConfig {
    public:
        static const uint DHT22_INPUT = 0;
        static const uint WIND_DIR_INPUT_0 = 1;
        static const uint WIND_DIR_INPUT_1 = 2;
        static const uint WIND_DIR_INPUT_2 = 3;
        static const uint WIND_DIR_INPUT_3 = 4;
        static const uint WIND_DIR_INPUT_4 = 5;
        static const uint WIND_DIR_INPUT_5 = 6;
        static const uint WIND_DIR_INPUT_6 = 7;
        static const uint WIND_DIR_INPUT_7 = 10;
        static const uint PRECIP_INPUT_1 = 11;
        static const uint PRECIP_INPUT_2 = 14;
        static const uint UART_TX_1 = 8;
        static const uint UART_RX_1 = 9;
        static const uint UART_TX_2 = 13;
        static const uint UART_RX_2 = 12;
};