#include "DHT22Sensor.h"

DHT22Sensor::DHT22Sensor(uint pin) : pin(pin), temperature(0.0f), humidity(0.0f), readSuccessful(false) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
}

bool DHT22Sensor::initialize() {
    temperature = 0.0f;
    humidity = 0.0f;
    readSuccessful = false;

    struct repeating_timer timer;
    add_repeating_timer_ms(-2000, readTemperatureAndHumidity_callback, this, &timer);
    return true;
}

bool DHT22Sensor::readTemperatureAndHumidity_callback(struct repeating_timer *t) {
    static_cast<DHT22Sensor *>(t->user_data)->readTemperatureAndHumidity();
    return true;
}

bool DHT22Sensor::update() {
    return readSuccessful;
}

float DHT22Sensor::getTemperature() {
    return temperature;
}

float DHT22Sensor::getHumidity() {
    return humidity;
}

bool DHT22Sensor::readTemperatureAndHumidity() {
    temperature = 25.0f;
    humidity = 50.0f;
    readSuccessful = true;
    return readSuccessful;
}
