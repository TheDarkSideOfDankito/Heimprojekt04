cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME Heimprojekt04)
project(${PROJECT_NAME})

set(${CMAKE_PROJECT_NAME}_SRCS Heimprojekt04.cpp pinMappings.h pinControl.h pinControl.cpp util.h util.cpp interrupts.h interrupts.cpp FailureMemory.h FailureMemory.cpp)

set(${PROJECT_NAME}_BOARD uno)
set(${PROJECT_NAME}_PORT /dev/ttyACM0)

generate_arduino_firmware(${CMAKE_PROJECT_NAME})
