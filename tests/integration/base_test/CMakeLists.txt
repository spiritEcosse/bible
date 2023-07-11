project(base_test LANGUAGES CXX)

set(RESOURCES include/entitybase.h include/basetest.h include/modeljsontest.h
              src/basetest.cpp src/modeljsontest.cpp src/entitybase.cpp)

add_library(${PROJECT_NAME} STATIC ${RESOURCES})

if(Qt6_FOUND)
  find_package(
    Qt6
    COMPONENTS Core Test Network Sql
    REQUIRED)
  target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Qt6::Core
            Qt6::Test
            Qt6::Network
            Qt6::Sql
            QuaZip
            sqlite_orm
            dbmanager
            netmanager
            core
            modules)
else()
  find_package(
    Qt5
    COMPONENTS Core Test Network Sql
    REQUIRED)
  target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Qt5::Core
            Qt5::Test
            Qt5::Network
            QuaZip
            sqlite_orm
            dbmanager
            netmanager
            core
            modules)
endif()

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC include
  PRIVATE src)
