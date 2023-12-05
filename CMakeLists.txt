cmake_minimum_required(VERSION 3.12)
 
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
 
project(Picophonica C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
 
pico_sdk_init()

add_subdirectory(lib/RP2040-Keypad-Matrix keypad_matrix)

add_executable(${PROJECT_NAME}
        main.c
        )

target_sources(${PROJECT_NAME} PRIVATE
        lib/tinyusb-midi/usb_descriptors.c
)

target_include_directories(${PROJECT_NAME} PUBLIC
        .
        lib/tinyusb-midi/
)

target_link_libraries(${PROJECT_NAME}
        pico_stdlib
        hardware_irq
        hardware_pwm
        keypad_matrix
        tinyusb_device
        tinyusb_board
        pico_unique_id # used to set a serial id for the Midi instrument
        )

pico_add_extra_outputs(${PROJECT_NAME})

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)