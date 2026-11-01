#pragma once

#include <string>
#include "PinConfig.h"
#include "MagnetSensor/MagnetSensor.h"
#include "WindDirectionModule/WindDirectionModule.h"
#include "DHT22Sensor/DHT22Sensor.h"
#include "PrecipitationSensor/PrecipitationSensor.h"
#include "UARTCommunication/UARTCommunication.h"

struct WeatherData {
    float temperature = 0.0f;
    float humidity = 0.0f;
    float pressure = 0.0f;
    float windSpeed = 0.0f;
    float windDirection = -1.0f;
    float precipitation = 0.0f;
};

class WeatherStation {
public:
    DHT22Sensor temperatureAndHumiditySensor;
    WeatherStation();
    WeatherData getWeatherData() const;
    bool initialize();
    bool update();

private:
    WindDirectionModule windDirectionModule;
    WeatherData currentData{};
    UARTCommunication uart;

    bool initializeData();
    bool initializeSensors();
    bool initializeCommunication();
    bool initializeClock();
    bool updateSensors();
    void readSensors();
    bool sendData();
};

