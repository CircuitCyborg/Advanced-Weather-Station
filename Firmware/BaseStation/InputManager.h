#pragma once
#include "pico/stdlib.h"

class InputManager {
public:
    static const uint PIN_RE1   = 12; // RE1
    static const uint PIN_RE2   = 27; // RE2
    static const uint PIN_SW_IN = 28; // Diode sensing line

    // Drive pins: SW1, SW2, SW3, SW4, SW5_OUT
    const uint out_pins[5] = { 14, 15, 16, 17, 18 };

    void init() {
        gpio_init(PIN_SW_IN);
        gpio_set_dir(PIN_SW_IN, GPIO_IN);
        gpio_pull_down(PIN_SW_IN);

        for (uint pin : out_pins) {
            gpio_init(pin);
            gpio_set_dir(pin, GPIO_OUT);
            gpio_put(pin, 0);
        }

        gpio_init(PIN_RE1);
        gpio_set_dir(PIN_RE1, GPIO_IN);
        gpio_pull_up(PIN_RE1);

        gpio_init(PIN_RE2);
        gpio_set_dir(PIN_RE2, GPIO_IN);
        gpio_pull_up(PIN_RE2);
    }

    // Returns a 5-bit mask: bit 0=SW1, bit 1=SW2, bit 2=SW3, bit 3=SW4, bit 4=SW5
    uint8_t readButtons() {
        uint8_t pressed = 0;
        for (int i = 0; i < 5; i++) {
            gpio_put(out_pins[i], 1);
            sleep_us(5);
            if (gpio_get(PIN_SW_IN)) {
                pressed |= (1 << i);
            }
            gpio_put(out_pins[i], 0);
        }
        return pressed;
    }
};