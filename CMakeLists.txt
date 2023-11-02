cmake_minimum_required(VERSION 3.25)
project(weather_app)

set(CMAKE_CXX_STANDARD 17)

add_executable(weather_app main.cpp location.cpp location.h)

find_package(CURL REQUIRED)
target_link_libraries(weather_app PRIVATE CURL::libcurl)
