project(modules LANGUAGES CXX)

file(GLOB SOURCES include/*.h src/*.cpp)
add_library(${PROJECT_NAME} STATIC ${SOURCES})

if(Qt6_FOUND)
  find_package(
    Qt6
    COMPONENTS Core Network Sql
    REQUIRED)
  target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Qt6::Core
            Qt6::Quick
            Qt6::Network
            Qt6::Sql
            core
            dbmanager
            netmanager
            QuaZip
            sqlite_orm)
else()
  find_package(
    Qt5
    COMPONENTS Core Network Sql
    REQUIRED)
  target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Qt5::Core
            Qt5::Quick
            Qt5::Network
            core
            dbmanager
            netmanager
            QuaZip
            sqlite_orm)
endif()

# if (${SAILFISH}) find_package(QuaZip5)
# include_directories(${QUAZIP_INCLUDE_DIRS})
# include_directories(${SQLITE_ORM_INCLUDE_DIR})
# target_link_libraries(${PROJECT_NAME} PRIVATE ${QUAZIP_LIBRARIES}) else()
# target_link_libraries(${PROJECT_NAME} PRIVATE QuaZip) endif()

# target_link_libraries(${PROJECT_NAME} PRIVATE QuaZip)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC include
  PRIVATE src)

# target_link_libraries(${PROJECT_NAME} PRIVATE core dbmanager netmanager)
target_compile_definitions(${PROJECT_NAME} PRIVATE MODULES_LIBRARY)
