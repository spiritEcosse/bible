project(tst_modules LANGUAGES CXX)

enable_testing()
if(Qt6_FOUND)
  find_package(
    Qt6
    COMPONENTS Test Network Qml Sql
    REQUIRED)
else()
  find_package(
    Qt5
    COMPONENTS Test Network Qml Sql
    REQUIRED)
endif()

file(GLOB SOURCES *.h *.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)

if(Qt6_FOUND)
  target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Qt6::Test
            Qt6::Network
            Qt6::Qml
            Qt6::Sql
            Qt6::Core
            core
            dbmanager
            netmanager
            modules
            base_test
            QuaZip)
else()
  target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Qt5::Test
            Qt5::Network
            Qt5::Qml
            core
            dbmanager
            netmanager
            modules
            base_test
            QuaZip)
endif()

add_custom_target(tests SOURCES ${SOURCES})

file(GLOB TEST_FILES main/*.cpp)
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
              Qt6::Core
              Qt6::Sql
              core
              dbmanager
              netmanager
              modules
              base_test
              QuaZip
              ${PROJECT_NAME})
  else()
    target_link_libraries(
      ${TEST_NAME}
      PRIVATE Qt5::Test
              Qt5::Network
              Qt5::Qml
              core
              dbmanager
              netmanager
              modules
              base_test
              QuaZip
              ${PROJECT_NAME})
  endif()
endforeach(TEST_FILE ${TEST_FILES})
