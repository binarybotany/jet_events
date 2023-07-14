cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)

project(jet_events
    VERSION 0.0.0.0 
    LANGUAGES CXX 
    DESCRIPTION "Jet Events library"
    HOMEPAGE_URL "https://www.github.com/binarybotany/jet_events")

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(jet_events INTERFACE)

target_include_directories(jet_events INTERFACE 
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")