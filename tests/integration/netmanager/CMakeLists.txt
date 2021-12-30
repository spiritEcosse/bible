project(tst_modules LANGUAGES CXX)

enable_testing()

find_package(Qt5 COMPONENTS Test Network Qml Sql REQUIRED)

file( GLOB TEST_FILES *.cpp )
foreach( TEST_FILE ${TEST_FILES} )
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable( ${TEST_NAME} ${TEST_FILE} )
    add_test(${TEST_NAME} ${TEST_NAME})

    target_link_libraries(${TEST_NAME} PRIVATE
        Qt5::Test Qt5::Network Qt5::Qml Qt5::Sql core netmanager modules dbmanager)
endforeach( TEST_FILE ${TEST_FILES} )
