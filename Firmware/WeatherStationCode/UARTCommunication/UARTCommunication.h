#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include <string>
#include "PinConfig.h"

extern PinConfig pinConfig;

class UARTCommunication {
public:
    bool initialize();
    bool sendData(const std::string& data);
    std::string receiveData();
private:
};