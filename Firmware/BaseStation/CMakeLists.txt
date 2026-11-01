cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.1)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()

set(PICO_BOARD pico CACHE STRING "Board type")
include(pico_sdk_import.cmake)

project(BaseStation C CXX ASM)
pico_sdk_init()

add_executable(BaseStation
    main.cpp
    ST7735/ST7735.cpp
    WeatherScreen/WeatherScreen.cpp
)

pico_set_program_name(BaseStation "BaseStation")
pico_set_program_version(BaseStation "0.1")

pico_enable_stdio_uart(BaseStation 0)
pico_enable_stdio_usb(BaseStation 1)

target_include_directories(BaseStation PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/ST7735
    ${CMAKE_CURRENT_LIST_DIR}/WeatherScreen
)

target_link_libraries(BaseStation
    pico_stdlib
    hardware_spi
    hardware_uart
    hardware_gpio
)

pico_add_extra_outputs(BaseStation)