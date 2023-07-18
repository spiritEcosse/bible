message(STATUS "========================Start configure ${LIB_QUAZIP}========================")

if (Qt6_FOUND)
    set(QUAZIP_QT_MAJOR_VERSION 6 CACHE STRING "Qt version to use 6")
endif()

add_subdirectory(3rdparty/${LIB_QUAZIP})
set(QUAZIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_QUAZIP}/${LIB_QUAZIP}
        CACHE PATH "quazip include directory")

message(STATUS "========================End configure ${LIB_QUAZIP}==========================")
