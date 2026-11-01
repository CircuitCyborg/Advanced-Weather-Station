#pragma once
#include "ST7735.h"
#include "WeatherData.h"

class WeatherScreen {
public:
    explicit WeatherScreen(ST7735& tft);

    void init();
    void update(const WeatherData& data);

private:
    ST7735& _tft;
    WeatherData _lastData;
    bool _firstDraw = true;

    void drawStaticLabels();
    void drawCompassNeedle(float deg, uint16_t color);
    void drawThermometerIcon(int x, int y);
    void drawDropletIcon(int x, int y);
    void drawWindBarChart(float speed_mph);
};