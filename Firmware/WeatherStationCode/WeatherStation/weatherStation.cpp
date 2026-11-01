#include "WeatherStation/WeatherStation.h"

PinConfig pinConfig;

WeatherStation::WeatherStation()
    : temperatureAndHumiditySensor(pinConfig.DHT22_INPUT),
      precipitationSensor(pinConfig.PRECIP_INPUT_1, 0.25f) {
}

WeatherData WeatherStation::getWeatherData() const {
    return currentData;
}

bool WeatherStation::initialize() {
    bool dataInitialized = initializeData();
    bool sensorsInitialized = initializeSensors();
    bool communicationInitialized = initializeCommunication();
    bool clockInitialized = initializeClock();
    return dataInitialized && sensorsInitialized && communicationInitialized && clockInitialized;
}

bool WeatherStation::update() {
    bool sensorUpdated = updateSensors();
    readSensors();
    bool dataSent = sendData();
    return sensorUpdated && dataSent;
}

bool WeatherStation::initializeData() {
    return true;
}

bool WeatherStation::initializeSensors() {
    bool windDirectionInitialized = windDirectionModule.initialize();
    bool temperatureAndHumidityInitialized = temperatureAndHumiditySensor.initialize();
    bool precipInitialized = precipitationSensor.initialize();
    return windDirectionInitialized && temperatureAndHumidityInitialized && precipInitialized;
}

bool WeatherStation::initializeCommunication() {
    return uart.initialize();
}

bool WeatherStation::initializeClock() {
    return true;
}

bool WeatherStation::updateSensors() {
    bool windDirectionUpdated = windDirectionModule.update();
    bool temperatureAndHumidityUpdated = temperatureAndHumiditySensor.update();
    bool precipUpdated = precipitationSensor.update();
    return windDirectionUpdated && temperatureAndHumidityUpdated && precipUpdated;
}

void WeatherStation::readSensors() {
    currentData.windDirection = windDirectionModule.getWindDirection();
    currentData.temperature = temperatureAndHumiditySensor.getTemperature();
    currentData.humidity = temperatureAndHumiditySensor.getHumidity();
    currentData.precipitation = precipitationSensor.getTotalRainfallInches();
}

bool WeatherStation::sendData() {
    std::string data = 
        "Temp=" + std::to_string(currentData.temperature) +
        ",HUM=" + std::to_string(currentData.humidity) +
        ",DIR=" + std::to_string(currentData.windDirection) +
        ",RAIN=" + std::to_string(currentData.precipitation) +
        "\n";

    return uart.sendData(data);
}