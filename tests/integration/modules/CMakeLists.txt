project(tst_modules LANGUAGES CXX)

enable_testing()
find_package(Qt5 COMPONENTS Test Network Qml Sql REQUIRED)
file(GLOB SOURCES *.h *.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Test Qt5::Network Qt5::Qml Qt5::Sql core dbmanager netmanager modules base_test QuaZip)

add_custom_target(tests SOURCES ${SOURCES})

file( GLOB TEST_FILES main/*.cpp )
foreach( TEST_FILE ${TEST_FILES} )
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable( ${TEST_NAME} ${TEST_FILE})
    add_test(${TEST_NAME} ${TEST_NAME})
    target_link_libraries(${TEST_NAME} PRIVATE
        Qt5::Test Qt5::Network Qt5::Qml Qt5::Sql core dbmanager netmanager modules base_test QuaZip ${PROJECT_NAME})
endforeach( TEST_FILE ${TEST_FILES} )
