cmake_minimum_required(VERSION 3.5)

project(bible LANGUAGES CXX)

#message("CMAKE_PROJECT_VERSION = ${CMAKE_PROJECT_VERSION}")
message("CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package (Qt5 COMPONENTS Widgets Core Network Qml Gui Quick REQUIRED)

execute_process (
    COMMAND bash -c "awk -F= '$1==\"ID\" { print $2 ;}' /etc/os-release"
    OUTPUT_VARIABLE PLATFORM
)
set ( SAILFISH FALSE )

if ("${PLATFORM}" MATCHES "sailfishos")
    set ( SAILFISH TRUE )
endif()

set(SOURCE src/${PROJECT_NAME}.cpp)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
    FILE(GLOB_RECURSE QML_FILES "${CMAKE_SOURCE_DIR}/*.qml")
    add_custom_target(qml SOURCES ${QML_FILES} ${SOURCE})
endif()

include(3rdparty/3rdparty.cmake)

set(QT_5_12_VERSION 5.12.0)

if(Qt5_VERSION VERSION_GREATER_EQUAL QT_5_12_VERSION)
    find_package (Qt5 COMPONENTS QuickCompiler REQUIRED)
endif()

if ( ${SAILFISH} )
    include(FindPkgConfig)
    pkg_search_module(SAILFISH sailfishapp REQUIRED)

    if(Qt5_VERSION VERSION_GREATER_EQUAL QT_5_12_VERSION)
        qtquick_compiler_add_resources(RESOURCES qml/qml.qrc)
    else()
        qt5_add_resources(RESOURCES qml/qml.qrc)
    endif()

    add_executable(${PROJECT_NAME} ${SOURCE} ${RESOURCES})
    set_property(SOURCE qrc_qml.cpp PROPERTY SKIP_AUTOGEN ON)

    target_include_directories(${PROJECT_NAME} PRIVATE
        $<BUILD_INTERFACE:
        ${SAILFISH_INCLUDE_DIRS}
        ${SQLITE_ORM_INCLUDE_DIR}
    >)
    target_link_libraries(${PROJECT_NAME}
        Qt5::Network
        Qt5::Quick
        Qt5::Widgets
        ${SAILFISH_LDFLAGS}
        modules
        dbmanager
        core
        netmanager
        sqlite_orm
    )

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
endif()

set(CMAKE_AUTOUIC ON)
add_subdirectory(src)

add_library(tag SHARED IMPORTED)
set_target_properties(tag PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/build/taglib/lib/libtag.so)

#ExternalProject_Add(quickdownload
#                    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/quickdownload
#                    URL https://github.com/Larpon/QuickDownload.git
#                    DOWNLOAD_DIR ${QGLViewer_DSTDIR}
#                    BUILD_IN_SOURCE 1
#                    CONFIGURE_COMMAND qmake quickdownload.pro PREFIX=${CMAKE_CURRENT_BINARY_DIR}/3rdparty/quickdownload
#                    BUILD_COMMAND make
#                   )

if ( BUILD_TESTING )
    enable_testing()

    if (CMAKE_BUILD_TYPE STREQUAL "Coverage")
      set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/.travis/cmake)
      include(CodeCoverage)
#      setup_target_for_coverage(${PROJECT_NAME}_coverage runUnitTests coverage)

      SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
      SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
    endif() #CMAKE_BUILD_TYPE STREQUAL "Coverage"

    add_subdirectory(tests)
endif()
