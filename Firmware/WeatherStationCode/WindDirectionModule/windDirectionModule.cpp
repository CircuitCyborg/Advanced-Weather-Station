#include "WindDirectionModule.h"

WindDirectionModule::WindDirectionModule() : magnetSensor{
    MagnetSensor(pinConfig.WIND_DIR_INPUT_0),
    MagnetSensor(pinConfig.WIND_DIR_INPUT_1),
    MagnetSensor(pinConfig.WIND_DIR_INPUT_2),
    MagnetSensor(pinConfig.WIND_DIR_INPUT_3),
    MagnetSensor(pinConfig.WIND_DIR_INPUT_4),
    MagnetSensor(pinConfig.WIND_DIR_INPUT_5),
    MagnetSensor(pinConfig.WIND_DIR_INPUT_6),
    MagnetSensor(pinConfig.WIND_DIR_INPUT_7)
}, windDirection(-1) {
}

bool WindDirectionModule::initialize() {
    windDirection = -1;
    return true;
}

bool WindDirectionModule::update() {
    for (int i = 0; i < 8; i++) {
        magnetSensor[i].update();
    }
    return readWindDirection();
}

float WindDirectionModule::getWindDirection() {
    return windDirection;
}

bool WindDirectionModule::getWindDirection() {
    windDirection = -1;
    for (int i = 0; i < 8; i++) {
        if (magnetSensor[i].getMagnetDetected()) {
            windDirection = i * 45;
            break;
        }
    }
    return windDirection != -1;
}
