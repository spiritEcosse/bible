find_package(Git QUIET)

#set(REBUILD_CURL On)
#set(REBUILD_NGHTTP2 On)
option(USE_BUNDLED_DEPS "Enable bundled dependencies instead of using the system ones" ON)
option(REBUILD_OPENSSL OFF)
option(REBUILD_AUTOCONF OFF)
option(REBUILD_AUTOMAKE OFF)
option(REBUILD_LIBTOOL OFF)
option(REBUILD_CURL OFF)
option(REBUILD_NGHTTP2 OFF)
set(LIB_AUTOCONF autoconf)
set(LIB_NGHTTP2 nghttp2)
set(LIB_AUTOMAKE automake)
set(LIBTOOL libtool)
set(LIB_CURL curl)
set(LIB_OPENSSL openssl)
set(LIB_SQLITE_ORM sqlite_orm)
set(LIB_QUAZIP quazip)
include(ProcessorCount)
ProcessorCount(CORES)

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    message(STATUS "========================Submodule update========================")
    execute_process(COMMAND bash -c "git submodule init")
    execute_process(COMMAND bash -c "tree 3rdparty")
    execute_process(COMMAND bash -c "git submodule foreach -q --recursive 'git checkout $(git config -f $toplevel/.gitmodules submodule.$name.branch || echo master)'"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with : ${GIT_SUBMOD_RESULT}, please checkout submodules")
    else()
        include(${LIB_QUAZIP})
        include(${LIB_SQLITE_ORM})
        include(${LIB_CURL})
    endif()
endif()
