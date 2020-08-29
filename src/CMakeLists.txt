project(src LANGUAGES CXX)

set(SOURCE main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

find_package(Qt5 COMPONENTS Core REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core)

add_subdirectory(core)
add_subdirectory(modules)
add_subdirectory(dbmanager)
add_subdirectory(netmanager)
