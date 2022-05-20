message(STATUS "========================Start configure ${LIB_QUAZIP}========================")

if (Qt6_FOUND)
    set(QUAZIP_QT_MAJOR_VERSION 6 CACHE STRING "Qt version to use 6")
endif()

#execute_process(COMMAND bash -c "sed -i '' -e '/add_subdirectory(qztest EXCLUDE_FROM_ALL)/ s/^#*/#/' 3rdparty/quazip/CMakeLists.txt"
#                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}) # need remove , need for build in travis
add_subdirectory(3rdparty/${LIB_QUAZIP})
set(QUAZIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_QUAZIP}/${LIB_QUAZIP}
        CACHE PATH "quazip include directory")

#if(NOT EXISTS "${PROJECT_SOURCE_DIR}/3rdparty/quazip/CMakeLists.txt")
#    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
#endif()

message(STATUS "========================End configure ${LIB_QUAZIP}==========================")
