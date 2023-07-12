#execute_process(COMMAND bash -c "xcrun --show-sdk-path" RESULT_VARIABLE CMAKE_OSX_SYSROOT)
cmake_minimum_required(VERSION 3.20)

project(bible LANGUAGES CXX)

# message("CMAKE_PROJECT_VERSION = ${CMAKE_PROJECT_VERSION}")
message("CMAKE_CXX_COMPILER_VERSION : ${CMAKE_CXX_COMPILER_VERSION}")
message("CMAKE_CXX_COMPILER : ${CMAKE_CXX_COMPILER}")
option(CMAKE_GENERATOR "Ninja")
message("CMAKE_OSX_SYSROOT:  ${CMAKE_OSX_SYSROOT}")
message("CMAKE_GENERATOR : ${CMAKE_GENERATOR}")
# * gcc -v && g++ -v && cmake --version
message("PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message("CMAKE_MAKE_PROGRAM : ${CMAKE_MAKE_PROGRAM}")

execute_process(COMMAND bash -c "c++ --version"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        RESULT_VARIABLE C++_VERSION_RESULT)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

find_package(Qt6 COMPONENTS Widgets)
if(NOT Qt6_FOUND)
  find_package(
    Qt5
    COMPONENTS Widgets
               Core
               Network
               Qml
               Gui
               Quick
               Sql
    REQUIRED)
else()
  find_package(
    Qt6
    COMPONENTS Widgets
               Core
               Network
               Qml
               Gui
               Quick
               QuickControls2
               Sql
    REQUIRED)
endif()

find_package(ZLIB)
set(QT_5_12_VERSION 5.12.0)

if(Qt5_VERSION VERSION_GREATER_EQUAL QT_5_12_VERSION)
  find_package(
    Qt5
    COMPONENTS QuickCompiler
    REQUIRED)
endif()

execute_process(
  COMMAND bash -c "awk -F= '$1==\"ID\" { print $2 ;}' /etc/os-release"
  OUTPUT_VARIABLE PLATFORM)

if("${PLATFORM}" MATCHES "sailfishos")
  set(SAILFISH TRUE)
endif()

if(CMAKE_VS_WINCE_VERSION)
  set(ARCH "${MSVC_C_ARCHITECTURE_ID}")
else()
  set(ARCH "${CMAKE_HOST_SYSTEM_PROCESSOR}")

  message("CMAKE_HOST_SYSTEM_PROCESSOR : ${CMAKE_HOST_SYSTEM_PROCESSOR}")
  set(TARGET_CURL "${ARCH}-linux")

  if(SAILFISH)
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "i486")
      set(ARCH "elf")
    endif()
  endif()

  if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "armv7l")
    set(TARGET_OPENSSL "linux-armv4")
  else()
    set(TARGET_OPENSSL "linux-${ARCH}")
  endif()
endif()

message("ARCH : ${ARCH}")
set(TARGET "linux-${ARCH}")

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(MACOSX TRUE)
  set(TARGET "darwin64-${ARCH}-cc")
  add_definitions(-DMACOSX)

  if(${ARCH} MATCHES "arm64")
    set(TARGET_CURL "arm")
    set(TARGET_OPENSSL "${TARGET}")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    # set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE INTERNAL "" FORCE)
    message("CMAKE_OSX_ARCHITECTURES: ${CMAKE_OSX_ARCHITECTURES}")
  endif()
endif()
# FILE(GLOB_RECURSE QML_FILES "${CMAKE_SOURCE_DIR}/*.qml") add_custom_target(qml
# SOURCES ${QML_FILES})

message("TARGET : ${TARGET}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty")
list(APPEND CMAKE_MODULE_PATH
     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cmake-scripts")
include(3rdparty)

if(BUILD_TESTING)
  enable_testing()

  if(CODE_COVERAGE)
    include(code-coverage)
    add_code_coverage_all_targets(EXCLUDE "3rdparty/*" "tests/*")

    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
  endif()
endif()

#set(INCLUDE_SANITIZER OFF CACHE BOOL "")
#if (${INCLUDE_SANITIZER})
#  message("INCLUDE_SANITIZER : ${INCLUDE_SANITIZER}")
#  set(USE_SANITIZER "Address,Leak")
#  include(sanitizers)
#endif()

#include(link-time-optimization)
#link_time_optimization()
#message("CMAKE_INTERPROCEDURAL_OPTIMIZATION : ${CMAKE_INTERPROCEDURAL_OPTIMIZATION}")

set_property(SOURCE ${RESOURCES} PROPERTY SKIP_AUTOMOC ON)
set(SOURCE src/${PROJECT_NAME}.cpp ${RESOURCES})

if(Qt6_FOUND)
  qt_add_executable(${PROJECT_NAME} qml_multi_platform/qml.qrc qml/qml.qrc ${SOURCE})
  add_definitions(-DQt6_FOUND)
elseif(Qt5_VERSION VERSION_GREATER_EQUAL QT_5_12_VERSION)
  qtquick_compiler_add_resources(RESOURCES qml/qml.qrc)
  add_executable(${PROJECT_NAME} ${SOURCE})
else()
  qt5_add_resources(RESOURCES qml/qml.qrc)
  add_executable(${PROJECT_NAME} ${SOURCE})
endif()

if(${SAILFISH})
  include(FindPkgConfig)
  pkg_search_module(SAILFISH sailfishapp REQUIRED)

  add_definitions(-DSAILFISH)

  target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Qt5::Network
            Qt5::Quick
            Qt5::Widgets
            modules
            dbmanager
            core
            netmanager
            sqlite_orm
            ssl
            crypto
            curl
            ZLIB::ZLIB
            nghttp2
            dl
            ${SAILFISH_LDFLAGS})
  target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:
                                                     ${SAILFISH_INCLUDE_DIRS} >)

  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

  install(DIRECTORY qml DESTINATION share/${PROJECT_NAME})
  install(
    DIRECTORY translations
    DESTINATION share/${PROJECT_NAME}
    FILES_MATCHING
    PATTERN "*.qm")
  install(FILES ${PROJECT_NAME}.desktop DESTINATION share/applications)
  install(FILES icons/86x86/${PROJECT_NAME}.png
          DESTINATION share/icons/hicolor/86x86/apps)
  install(FILES icons/108x108/${PROJECT_NAME}.png
          DESTINATION share/icons/hicolor/108x108/apps)
  install(FILES icons/128x128/${PROJECT_NAME}.png
          DESTINATION share/icons/hicolor/128x128/apps)
  install(FILES icons/172x172/${PROJECT_NAME}.png
          DESTINATION share/icons/hicolor/172x172/apps)

  # Tell Qt Creator where the application executable(s) would be located on the
  # device.
  #
  # It is not necessary to list other deployables than executables (runtime
  # targets) here. The deployment process of Sailfish OS projects is opaque to
  # Qt Creator and the information contained in QtCreatorDeployment.txt is only
  # used to locate the executable associated with the active run configuration
  # on the device in order to run it.
  #
  # Search the Qt Creator Manual to learn about the QtCreatorDeployment.txt file
  # format.
  file(WRITE "${CMAKE_BINARY_DIR}/QtCreatorDeployment.txt"
       "${CMAKE_INSTALL_PREFIX}\n${CMAKE_BINARY_DIR}/${PROJECT_NAME}:bin\n")
else()
  if(Qt6_FOUND)
    target_link_libraries(
      ${PROJECT_NAME}
      PRIVATE Qt6::Network
              Qt6::Quick
              Qt6::QuickControls2
              Qt6::Sql
              Qt6::Widgets
              modules
              dbmanager
              core
              netmanager
              sqlite_orm
              ssl
              crypto
              curl
              ZLIB::ZLIB
              nghttp2
              dl)
  else()
    target_link_libraries(
      ${PROJECT_NAME}
      PRIVATE Qt5::Network
              Qt5::Quick
              Qt5::Widgets
              modules
              dbmanager
              core
              netmanager
              sqlite_orm
              ssl
              crypto
              curl
              ZLIB::ZLIB
              nghttp2
              dl)
  endif()
endif()

set(CMAKE_AUTOUIC ON)
add_subdirectory(src)

if(BUILD_TESTING)
  if(CODE_COVERAGE)
    target_code_coverage(core AUTO ALL)
    target_code_coverage(modules AUTO ALL)
    target_code_coverage(dbmanager AUTO ALL)
    target_code_coverage(netmanager AUTO ALL)
  endif()

  add_subdirectory(tests)
endif()

include(formatting)

file(GLOB_RECURSE ALL_CODE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp
     ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/tests/*.cpp
     ${PROJECT_SOURCE_DIR}/tests/*.h)

clang_format(format ${ALL_CODE_FILES})

file(GLOB_RECURSE CMAKE_FILES ${PROJECT_SOURCE_DIR}/src/CMakeLists.txt
     ${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt)

cmake_format(cmake-format ${CMAKE_FILES} CMakeLists.txt)

get_cmake_property(_variableNames VARIABLES)
list(SORT _variableNames)
foreach(_variableName ${_variableNames})
  message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
