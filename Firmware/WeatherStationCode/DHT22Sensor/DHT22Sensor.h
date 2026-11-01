#pragma once

#include "pico/stdlib.h"
#include "pico/time.h"

class DHT22Sensor{ //handles the DHT22 temp and humidity sensor
    public:
        bool initialize();
        static bool readTemperatureAndHumidity_callback(struct repeating_timer *t);
        bool update(),
        float getTemperature();
        float getHumidity();
        DHT22Sensor(uint pin);
    
    private:
        uint pin;
        float temperature;
        float humidity;
        bool readSuccessful;
        bool readTemperatureAndHumidity();
};