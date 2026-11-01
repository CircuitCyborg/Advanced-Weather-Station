
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Initialise pico_sdk from installed location

# == DO NOT EDIT BELOW THIS LINE ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.1)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if(EXISTS ${picovscode})
    include(${picovscode})
endif()
# =================================================================

set(PICO_BOARD pico CACHE STRING "Board type")

#Pull in Raspberry Pi Pico SDK
include(pico_sdk_import.cmake)

project(Station C CXX ASM)

#Init the Raspberry Pi Pico SDK
pico_sdk_init()

# add executable

add_executable(Station
    main.cpp
    WeatherStation/weatherStation.cpp
    MagnetSensor/magnetSensor.cpp
    WindDirectionModule/windDirectionModule.cpp
    DHT22Sensor/dht22Sensor.cpp
    PrecipitationSensor/PrecipitationSensor.cpp
    UARTCommunication/uartCommunication.cpp
)

pico_set_program_name(Station "Station")
pico_set_program_version(Station "0.1")

pico_enable_stdio_uart(Station 0)
pico_enable_stdio_usb(Station 0)

target_link_libraries(Station
        pico_stdlib)

target_include_directories(Station PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(Station
        hardware_spi
        hardware_i2c
        )

pico_add_extra_outputs(Station)
