cmake_minimum_required(VERSION 3.5)

project(DownloadModules LANGUAGES CXX)

find_package(Qt5Test REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

add_executable(DownloadModules tst_downloadmodules.cpp)
add_test(DownloadModules COMMAND DownloadModules)

target_link_libraries(DownloadModules PRIVATE Qt5::Test Qt5::Core Qt5::Sql)
