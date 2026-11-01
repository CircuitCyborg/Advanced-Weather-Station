#include "MagnetSensor.h"
#include "hardware/gpio.h"

MagnetSensor::MagnetSensor(uint pin) : pin(pin), magnetDetected(false) {
    gpio_init(pin)
    gpio_set_dir(pin, GPIO_IN);
};
bool MagnetSensor::getMagnetDetected(){
    return magnetDetected;
}
void MagnetSensor::update(){
    readMagnetSensor();
};
bool MagnetSensor::initialize(){
    magnetDetected = false;
    return !magnetDetected;
};
void MagnetSensor::readMagnetSensor() {
    magnetDetected = gpio_get(pin);
};