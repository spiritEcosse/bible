cmake_minimum_required(VERSION 3.5)

project(DownloadModules LANGUAGES CXX)

find_package(Qt5Test REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_testing()

set(SRC_PATH ../../src)
set(Headers
    ${SRC_PATH}/DownloadManager.h
    ${SRC_PATH}/ModulesGroupModel.h
    ${SRC_PATH}/ModulesModel.h
    ${SRC_PATH}/dbmanager.h
    ${SRC_PATH}/TextProgressBar.h
)

set(Sources
    ${SRC_PATH}/DownloadManager.cpp
    ${SRC_PATH}/ModulesGroupModel.cpp
    ${SRC_PATH}/ModulesModel.cpp
    ${SRC_PATH}/dbmanager.cpp
    ${SRC_PATH}/TextProgressBar.cpp
)

add_executable(DownloadModules tst_downloadmodules.cpp main.cpp ${Sources} ${Headers})
add_test(DownloadModules COMMAND DownloadModules)

target_link_libraries(DownloadModules PRIVATE
    Qt5::Test
    Qt5::Core
    Qt5::Sql
    Qt5::Network
    quazip5
)
