#pragma once

#include "pico/stdlib.h"
#include "../MagnetSensor/MagnetSensor.h"
#include "../PinConfig.h"

extern PinConfig pinConfig;

class PrecipitationSensor {
public:
    // tip_capacity_inches: calibration constant (0.25 in per tip)
    explicit PrecipitationSensor(uint pin, float tip_capacity_inches = 0.25f);

    bool initialize();
    bool update();

    //Returns cumulative rainfall in inches
    float getTotalRainfallInches() const;

    // Returns total tip count
    uint32_t getTipCount() const;

    // Resets accumulation counter
    void resetRainfall();

private:
    MagnetSensor _magnetSensor;
    float _tipCapacity;
    uint32_t _tipCount;
    bool _lastState;
    uint64_t _lastTripTimeUs;
    static const uint64_t DEBOUNCE_LOCKOUT_US = 200000; // 200ms debounce
};