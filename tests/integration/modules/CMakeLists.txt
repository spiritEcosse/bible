project(tst_modules LANGUAGES CXX)

enable_testing()

find_package(Qt5 COMPONENTS Test Network REQUIRED)

set(QT_TEST_SERVER 1)
file( GLOB TEST_FILES *.cpp )
foreach( TEST_FILE ${TEST_FILES} )
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable( ${TEST_NAME} ${TEST_FILE} )
    add_test(${TEST_NAME} ${TEST_NAME})
    if (${SAILFISH})
        find_package(QuaZip5)
        include_directories(${QUAZIP_INCLUDE_DIRS})
        target_link_libraries(${TEST_NAME} PRIVATE ${QUAZIP_LIBRARIES})
    else()
        target_link_libraries(${TEST_NAME} PRIVATE QuaZip)
    endif()

    include_directories(${SQLITE_ORM_INCLUDE_DIR})
    target_link_libraries(${TEST_NAME} PRIVATE
        Qt5::Test Qt5::Network core dbmanager netmanager modules)
endforeach( TEST_FILE ${TEST_FILES} )
