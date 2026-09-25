cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(134_led_module C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(134_led_module
    main.c
    led/led.c
    logging/log.c
)

target_include_directories(134_led_module PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/led
    ${CMAKE_CURRENT_SOURCE_DIR}/logging
)

target_link_libraries(134_led_module
    pico_stdlib
)

pico_enable_stdio_usb(134_led_module 1)
pico_enable_stdio_uart(134_led_module 0)

pico_add_extra_outputs(134_led_module)