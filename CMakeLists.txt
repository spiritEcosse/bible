cmake_minimum_required(VERSION 3.5)

project(bible LANGUAGES CXX)

execute_process (
    COMMAND bash -c "awk -F= '$1==\"ID\" { print $2 ;}' /etc/os-release"
    OUTPUT_VARIABLE PLATFORM
)
set ( SAILFISH FALSE )

if ("${PLATFORM}" MATCHES "sailfishos")
    set ( SAILFISH TRUE )
endif()

if ( ${SAILFISH} )
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
        "${CMAKE_INSTALL_PREFIX}\n${CMAKE_BINARY_DIR}/cmakesample:bin\n")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

include(3rdparty/3rdparty.cmake)
add_subdirectory(src)
add_subdirectory(tests)
