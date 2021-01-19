project(src LANGUAGES CXX)

set(SOURCE main.cpp)

find_package (Qt5 COMPONENTS Widgets Core Network Qml Gui Quick REQUIRED)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
    FILE(GLOB_RECURSE QML_FILES "${CMAKE_SOURCE_DIR}/*.qml")
    add_custom_target(qml SOURCES ${QML_FILES})

    add_executable(${PROJECT_NAME} ${SOURCE})
endif()

if ( ${SAILFISH} )
    include(FindPkgConfig)
    pkg_search_module(SAILFISH sailfishapp REQUIRED)

    qt5_add_resources(RESOURCES qml/qml.qrc)
    add_executable(${PROJECT_NAME} ${SOURCE} ${RESOURCES})

    set_property(SOURCE qrc_qml.cpp PROPERTY SKIP_AUTOGEN ON)

    target_include_directories(${PROJECT_NAME} PRIVATE
        $<BUILD_INTERFACE:
        ${SAILFISH_INCLUDE_DIRS}
    >)
    target_link_libraries(${PROJECT_NAME}
        Qt5::Quick
        Qt5::Widgets
        Qt5::Qml
        ${SAILFISH_LDFLAGS}
    )

    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
    )
endif()


add_subdirectory(core)
add_subdirectory(modules)
add_subdirectory(dbmanager)
add_subdirectory(netmanager)
