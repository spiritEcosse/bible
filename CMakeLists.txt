cmake_minimum_required(VERSION 3.19)

project(bible LANGUAGES CXX)

#message("CMAKE_PROJECT_VERSION = ${CMAKE_PROJECT_VERSION}")
message("CMAKE_CXX_COMPILER_VERSION : ${CMAKE_CXX_COMPILER_VERSION}")
#- gcc -v && g++ -v && cmake --version
message("PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message("Qt5_VERSION: ${Qt5_VERSION}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

#list(APPEND CMAKE_PREFIX_PATH "/Users/ihor/Qt5.6.3/5.6.3/clang_64/lib/cmake/Qt5/")
#list(APPEND CMAKE_PREFIX_PATH "/Users/ihor/Qt5.6.3/5.6.3/clang_64/bin/")

find_package (Qt5 COMPONENTS Widgets Core Network Qml Gui Quick Sql REQUIRED)
find_package(ZLIB)
set(QT_5_12_VERSION 5.12.0)

if(Qt5_VERSION VERSION_GREATER_EQUAL QT_5_12_VERSION)
    find_package (Qt5 COMPONENTS QuickCompiler REQUIRED)
endif()

execute_process (
    COMMAND bash -c "awk -F= '$1==\"ID\" { print $2 ;}' /etc/os-release"
    OUTPUT_VARIABLE PLATFORM
)

if ("${PLATFORM}" MATCHES "sailfishos")
    set ( SAILFISH TRUE )
endif()

if (CMAKE_VS_WINCE_VERSION)
    set(ARCH "${MSVC_C_ARCHITECTURE_ID}")
else()
    if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "armv7l")
        set(ARCH "aarch64")
    else()
        set(ARCH "${CMAKE_HOST_SYSTEM_PROCESSOR}")
    endif()
    message("CMAKE_HOST_SYSTEM_PROCESSOR : ${CMAKE_HOST_SYSTEM_PROCESSOR}")
    set(TARGET_CURL "${ARCH}-linux")

    if(SAILFISH)
        if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "i486")
            set(ARCH "elf")
        endif()
    endif()
endif()

message("ARCH : ${ARCH}")
set(TARGET "linux-${ARCH}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
    set(TARGET "darwin64-${ARCH}-cc")
    set(TARGET_CURL "arm")
endif()
#FILE(GLOB_RECURSE QML_FILES "${CMAKE_SOURCE_DIR}/*.qml")
#add_custom_target(qml SOURCES ${QML_FILES})

message("TARGET : ${TARGET}")

execute_process(COMMAND bash -c "\
    ls -la 3rdparty"
)
set(LIB_NGHTTP2 nghttp2)
set(LIB_CURL curl)
set(LIB_OPENSSL openssl)

set(OPENSSL_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_OPENSSL}")
set(OPENSSL_INSTALL_DIR "${OPENSSL_BUNDLE_DIR}/build")
set(OPENSSL_LIB_DIR "${OPENSSL_INSTALL_DIR}/lib")

set(NGHTTP2_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_NGHTTP2}")
set(NGHTTP2_INSTALL_DIR "${NGHTTP2_BUNDLE_DIR}/build")
set(NGHTTP2_INCLUDE_DIR "${NGHTTP2_INSTALL_DIR}/include")
set(NGHTTP2_LIB_DIR "${NGHTTP2_INSTALL_DIR}/lib")

set(CURL_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_CURL}")
set(CURL_INSTALL_DIR "${CURL_BUNDLE_DIR}/build")
set(CURL_INCLUDE_DIR "${CURL_INSTALL_DIR}/include")
set(CURL_LIB_DIR "${CURL_INSTALL_DIR}/lib")
message("CURL_INCLUDE_DIR : ${CURL_INCLUDE_DIR}")
include_directories("${CURL_INCLUDE_DIR}")
execute_process(COMMAND bash -c "\
    ls -la ${CURL_INCLUDE_DIR}"
)

link_directories("${OPENSSL_LIB_DIR}")
link_directories("${NGHTTP2_LIB_DIR}")
link_directories("${CURL_LIB_DIR}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty")
include(3rdparty)

if(Qt5_VERSION VERSION_GREATER_EQUAL QT_5_12_VERSION)
    qtquick_compiler_add_resources(RESOURCES qml/qml.qrc)
else()
    qt5_add_resources(RESOURCES qml/qml.qrc)
endif()
set_property(SOURCE ${RESOURCES} PROPERTY SKIP_AUTOMOC ON)

set(SOURCE src/${PROJECT_NAME}.cpp ${RESOURCES})
add_executable(${PROJECT_NAME} ${SOURCE})


if ( ${SAILFISH} )
    include(FindPkgConfig)
    pkg_search_module(SAILFISH sailfishapp REQUIRED)

    add_definitions(-DSAILFISH)

    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt5::Network
        Qt5::Quick
        Qt5::Sql
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
        ${SAILFISH_LDFLAGS}
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        $<BUILD_INTERFACE:
        ${SAILFISH_INCLUDE_DIRS}
    >)

    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
    )

    install(DIRECTORY qml
        DESTINATION share/${PROJECT_NAME}
    )
    install(DIRECTORY translations
        DESTINATION share/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.qm"
    )
    install(FILES ${PROJECT_NAME}.desktop
        DESTINATION share/applications
    )
    install(FILES icons/86x86/${PROJECT_NAME}.png
        DESTINATION share/icons/hicolor/86x86/apps
    )
    install(FILES icons/108x108/${PROJECT_NAME}.png
        DESTINATION share/icons/hicolor/108x108/apps
    )
    install(FILES icons/128x128/${PROJECT_NAME}.png
        DESTINATION share/icons/hicolor/128x128/apps
    )
    install(FILES icons/172x172/${PROJECT_NAME}.png
        DESTINATION share/icons/hicolor/172x172/apps
    )

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
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt5::Network
        Qt5::Quick
        Qt5::Sql
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
    )
endif()

if ( BUILD_TESTING )
    enable_testing()
    option(QUAZIP_ENABLE_TESTS "Enable quazip tests" OFF)
    option(CODE_COVERAGE "Enable coverage reporting" OFF)

    if (CODE_COVERAGE)
#        list(APPEND CMAKE_PREFIX_PATH "/usr/local/Cellar/llvm/12.0.1/lib/cmake/llvm/")
#        list(APPEND CMAKE_PREFIX_PATH "/usr/local/Cellar/llvm/12.0.1/bin/")
        find_package(LLVM REQUIRED CONFIG)

        message(${LLVM_INCLUDE_DIRS})
        message(${LLVM_DEFINITIONS})

        message(STATUS "Found LLVM version : ${LLVM_PACKAGE_VERSION}")
        message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

        set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/.travis/cmake)
        include(CodeCoverage)
        add_code_coverage_all_targets(EXCLUDE 3rdparty/* tests/*) # Adds the 'ccov-all' target set and sets it to exclude all files in test/ folders.

        SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
        SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    endif()
endif()

set(CMAKE_AUTOUIC ON)
add_subdirectory(src)

#add_library(tag SHARED IMPORTED)
#set_target_properties(tag PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/build/taglib/lib/libtag.so)

#ExternalProject_Add(quickdownload
#                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/quickdownload
#                    URL https://github.com/Larpon/QuickDownload.git
#                    DOWNLOAD_DIR ${QGLViewer_DSTDIR}
#                    BUILD_IN_SOURCE 1
#                    CONFIGURE_COMMAND qmake quickdownload.pro PREFIX=${CMAKE_CURRENT_BINARY_DIR}/3rdparty/quickdownload
#                    BUILD_COMMAND make
#                   )

if ( BUILD_TESTING )
    add_subdirectory(tests)
endif()
