project(src LANGUAGES CXX)

set(SOURCE main.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

if ( ${SAILFISH} )
    find_package (Qt5 COMPONENTS Core Network Qml Gui Quick REQUIRED)
    include(FindPkgConfig)
    pkg_search_module(SAILFISH sailfishapp REQUIRED)

    target_include_directories(${PROJECT_NAME} PRIVATE
        $<BUILD_INTERFACE:
        ${SAILFISH_INCLUDE_DIRS}
    >)
    target_link_libraries(${PROJECT_NAME}
        Qt5::Quick
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
