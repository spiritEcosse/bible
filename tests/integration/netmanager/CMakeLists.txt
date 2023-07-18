project(tst_modules LANGUAGES CXX)

enable_testing()

if(Qt6_FOUND)
  find_package(
    Qt6
    COMPONENTS Test Network Qml
    REQUIRED)
else()
  find_package(
    Qt5
    COMPONENTS Test Network Qml
    REQUIRED)
endif()

file(GLOB TEST_FILES *.cpp)
foreach(TEST_FILE ${TEST_FILES})
  get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
  add_executable(${TEST_NAME} ${TEST_FILE})
  add_test(${TEST_NAME} ${TEST_NAME})

  if(Qt6_FOUND)
    target_link_libraries(
      ${TEST_NAME}
      PRIVATE Qt6::Test
              Qt6::Network
              Qt6::Qml
              core
              netmanager
              modules
              dbmanager
              ssl
              crypto
              curl
              ZLIB::ZLIB
              nghttp2
              dl)
  else()
    target_link_libraries(
      ${TEST_NAME}
      PRIVATE Qt5::Test
              Qt5::Network
              Qt5::Qml
              core
              netmanager
              modules
              dbmanager
              ssl
              crypto
              curl
              ZLIB::ZLIB
              nghttp2
              dl)
  endif()
endforeach(TEST_FILE ${TEST_FILES})
