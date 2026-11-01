#pragma once
#include <cstdint>

struct WeatherData {
    float temp_f          = 75.0f;
    float humidity_pct    = 45.0f;
    float precip_rate_in  = 0.0f;
    float wind_speed_mph  = 13.0f;
    float wind_dir_deg    = 315.0f; // 0=N, 90=E, 180=S, 270=W

    float avg_temp_24h    = 79.0f;
    float weekly_rain_in  = 3.2f;
    uint8_t hours         = 13;
    uint8_t minutes       = 45;
};