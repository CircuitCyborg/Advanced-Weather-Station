#include "WeatherScreen.h"
#include <cstdio>
#include <cmath>

static const int COMPASS_CX = 28;
static const int COMPASS_CY = 34;
static const int COMPASS_R  = 20;

WeatherScreen::WeatherScreen(ST7735& tft) : _tft(tft) {}

void WeatherScreen::drawThermometerIcon(int x, int y) {
    _tft.drawFastVLine(x + 1, y, 8, TFT_LIGHTGREY);
    _tft.drawFastVLine(x + 3, y, 8, TFT_LIGHTGREY);
    _tft.fillCircle(x + 2, y + 10, 3, TFT_RED);
    _tft.fillRect(x + 2, y + 3, 1, 6, TFT_RED);
}

void WeatherScreen::drawDropletIcon(int x, int y) {
    _tft.fillCircle(x + 2, y + 4, 2, TFT_CYAN);
    _tft.drawLine(x, y + 4, x + 2, y, TFT_CYAN);
    _tft.drawLine(x + 4, y + 4, x + 2, y, TFT_CYAN);
}

void WeatherScreen::drawCompassNeedle(float deg, uint16_t color) {
    float rad = (deg - 90.0f) * (M_PI / 180.0f);
    int16_t nx = COMPASS_CX + (int16_t)(cos(rad) * (COMPASS_R - 3));
    int16_t ny = COMPASS_CY + (int16_t)(sin(rad) * (COMPASS_R - 3));
    int16_t tx = COMPASS_CX - (int16_t)(cos(rad) * (COMPASS_R / 2));
    int16_t ty = COMPASS_CY - (int16_t)(sin(rad) * (COMPASS_R / 2));

    _tft.drawLine(tx, ty, nx, ny, color);

    float rad_left = rad + 2.5f;
    float rad_right = rad - 2.5f;
    _tft.drawLine(nx, ny, nx + (int16_t)(cos(rad_left) * 4), ny + (int16_t)(sin(rad_left) * 4), color);
    _tft.drawLine(nx, ny, nx + (int16_t)(cos(rad_right) * 4), ny + (int16_t)(sin(rad_right) * 4), color);
}

void WeatherScreen::drawWindBarChart(float speed_mph) {
    int startX = 66;
    int baseY  = 124;
    int barW   = 8;
    int space  = 3;
    int heights[5]      = { 8, 15, 23, 31, 40 };
    float thresholds[5] = { 10.0f, 25.0f, 50.0f, 75.0f, 100.0f };

    for (int i = 0; i < 5; i++) {
        int x = startX + i * (barW + space);
        int h = heights[i];
        int y = baseY - h;

        _tft.drawFastHLine(x, y, barW, TFT_WHITE);
        _tft.drawFastHLine(x, baseY, barW, TFT_WHITE);
        _tft.drawFastVLine(x, y, h, TFT_WHITE);
        _tft.drawFastVLine(x + barW - 1, y, h, TFT_WHITE);

        uint16_t fill = (speed_mph >= thresholds[i]) ? TFT_WHITE : TFT_BG;
        _tft.fillRect(x + 1, y + 1, barW - 2, h - 1, fill);
    }
    _tft.drawString(110, baseY + 3, "100", TFT_LIGHTGREY, TFT_BG, 1);
}

void WeatherScreen::drawStaticLabels() {
    _tft.fillScreen(TFT_BG);

    // Compass dial
    _tft.drawCircle(COMPASS_CX, COMPASS_CY, COMPASS_R, TFT_WHITE);
    _tft.drawString(COMPASS_CX - 2, COMPASS_CY - COMPASS_R - 8, "N", TFT_WHITE, TFT_BG, 1);
    _tft.drawString(COMPASS_CX - 2, COMPASS_CY + COMPASS_R + 2, "S", TFT_WHITE, TFT_BG, 1);
    _tft.drawString(COMPASS_CX + COMPASS_R + 2, COMPASS_CY - 3, "E", TFT_WHITE, TFT_BG, 1);
    _tft.drawString(COMPASS_CX - COMPASS_R - 7, COMPASS_CY - 3, "W", TFT_WHITE, TFT_BG, 1);

    // Icons
    drawThermometerIcon(118, 8);
    drawDropletIcon(118, 48);

    // Dividers & Historical labels
    _tft.drawFastHLine(2, 68, 124, TFT_DARKGREY);
    _tft.drawString(4, 73, "24H AVG", TFT_LIGHTGREY, TFT_BG, 1);
    _tft.drawString(4, 114, "1 week", TFT_LIGHTGREY, TFT_BG, 1);
}

void WeatherScreen::init() {
    drawStaticLabels();
    _firstDraw = true;
}

void WeatherScreen::update(const WeatherData& data) {
    char buf[16];

    if (_firstDraw) {
        drawCompassNeedle(data.wind_dir_deg, TFT_CYAN);
        drawWindBarChart(data.wind_speed_mph);
    } else {
        if (std::abs(data.wind_dir_deg - _lastData.wind_dir_deg) > 1.0f) {
            drawCompassNeedle(_lastData.wind_dir_deg, TFT_BG);
            drawCompassNeedle(data.wind_dir_deg, TFT_CYAN);
        }
        if (std::abs(data.wind_speed_mph - _lastData.wind_speed_mph) > 0.5f) {
            drawWindBarChart(data.wind_speed_mph);
        }
    }

    if (_firstDraw || data.temp_f != _lastData.temp_f) {
        snprintf(buf, sizeof(buf), "%4.1fF", data.temp_f);
        _tft.drawString(56, 8, buf, TFT_WHITE, TFT_BG, 2);
    }

    if (_firstDraw || data.humidity_pct != _lastData.humidity_pct) {
        snprintf(buf, sizeof(buf), "%2.0f%%", data.humidity_pct);
        _tft.drawString(68, 28, buf, TFT_CYAN, TFT_BG, 2);
    }

    if (_firstDraw || data.precip_rate_in != _lastData.precip_rate_in) {
        snprintf(buf, sizeof(buf), "%3.1fin", data.precip_rate_in);
        _tft.drawString(60, 48, buf, TFT_WHITE, TFT_BG, 1);
    }

    if (_firstDraw || data.avg_temp_24h != _lastData.avg_temp_24h) {
        snprintf(buf, sizeof(buf), "%2.0fF", data.avg_temp_24h);
        _tft.drawString(8, 86, buf, TFT_WHITE, TFT_BG, 2);
    }

    if (_firstDraw || data.weekly_rain_in != _lastData.weekly_rain_in) {
        snprintf(buf, sizeof(buf), "%3.1fin", data.weekly_rain_in);
        _tft.drawString(6, 125, buf, TFT_WHITE, TFT_BG, 1);
    }

    if (_firstDraw || data.wind_speed_mph != _lastData.wind_speed_mph) {
        snprintf(buf, sizeof(buf), "%2.0fMPH", data.wind_speed_mph);
        _tft.drawString(68, 73, buf, TFT_WHITE, TFT_BG, 1);
    }

    if (_firstDraw || data.hours != _lastData.hours || data.minutes != _lastData.minutes) {
        snprintf(buf, sizeof(buf), "%02d:%02d", data.hours, data.minutes);
        _tft.drawString(34, 140, buf, TFT_GREEN, TFT_BG, 2);
    }

    _lastData = data;
    _firstDraw = false;
}