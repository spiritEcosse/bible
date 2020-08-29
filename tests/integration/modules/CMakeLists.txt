project(tst_modules LANGUAGES CXX)

enable_testing()

find_package(Qt5 COMPONENTS Test Sql Network REQUIRED)

set(QT_TEST_SERVER 1)
file( GLOB TEST_FILES *.cpp )
foreach( TEST_FILE ${TEST_FILES} )
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable( ${TEST_NAME} ${TEST_FILE} )
    add_test(${TEST_NAME} ${TEST_NAME})
    target_link_libraries(${TEST_NAME} PRIVATE
        Qt5::Test Qt5::Sql Qt5::Network core dbmanager netmanager modules QuaZip)
endforeach( TEST_FILE ${TEST_FILES} )
