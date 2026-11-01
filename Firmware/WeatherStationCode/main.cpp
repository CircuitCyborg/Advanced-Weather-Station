#include "PinConfig.h"/stdlib.h"
#include "iostream"
#include "WeatherStation/WeatherStation.h"

using namespace std;
int main() {
    station.initialize();
    
    while (1) {
        station.update();
        sleep_us(1000); // Sleep for 1ms
    }
    return 0;
}