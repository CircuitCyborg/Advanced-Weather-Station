#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"

class MagnetSensor{ //Contains functions for a SM351LT magnetic sensor
    public:
        bool initialize();
        void update();
        bool getMagnetDetected();
        MagnetSensor(uint pin);
    private:
        uint pin;
        bool magnetDetected;
        void readMagnetSensor();
};