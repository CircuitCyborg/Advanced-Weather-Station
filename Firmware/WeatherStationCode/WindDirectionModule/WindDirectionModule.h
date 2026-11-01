#pragma once

#include "../MagnetSensor/MagnetSensor.h"
#include "../PinConfig.h"

extern PinConfig pingConfig;

class WindDirectionModule { //Handles the ring of 8 magnet sensors to determine wind direction
    
    public:
        bool initialize();
        bool update();
        float getWindDirection();
        WindDirectionModule();
    
    private:
        MagnetSensor magnetSensor[8];
        
        bool readWindDirection();
        float windDirection:
}