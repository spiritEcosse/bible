cmake_minimum_required(VERSION 3.5)

project(bible LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_EXAMPLES OFF) # option BUILD_EXAMPLES available sinse with sqlite_orm==1.6

include(3rdparty/3rdparty.cmake)
add_subdirectory(src)
add_subdirectory(tests)
