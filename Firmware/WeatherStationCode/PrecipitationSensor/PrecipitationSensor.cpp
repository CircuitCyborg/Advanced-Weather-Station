#include "PrecipitationSensor.h"

PrecipitationSensor::PrecipitationSensor(uint pin, float tip_capacity_inches)
    : _magnetSensor(pin),
    _tipCapacity(tip_capacity_inches),
    _tipCount(0),
    _lastState(false),
    _lastTripTimeUs(0) {}

bool PrecipitationSensor::initialize() {
    _magnetSensor.initialize();
    _lastState = _magnetSensor.getMagnetDetected();
    _tipCount = 0;
    _lastTripTimeUs = to_us_since_boot(get_absolute_time());
    return true;
}

bool PrecipitationSensor::update() {
    _magnetSensor.update();
    bool currentState = _magnetSensor.getMagnetDetected();
    uint64_t now = to_us_since_boot(get_absolute_time());

    // Detect Magnet transition
    if (currentState && !_lastState) {
        if (now - _lastTripTimeUs >= DEBOUNCE_LOCKOUT_US) {
            _tipCount++;
            _lastTripTimeUs = now;
        }
    }

    _last State = currentState;
    return true;
}

float PrecipitationSensor::getTotalRainfallInches() const {
    return (float)_tipCount * _tipCapacity;
}

uint32_t PrecipitationSensor::getTipCount() const {
    return _tipCount;
}

void PrecipitationSensor::resetRainfall() {
    _tipCount = 0;
}