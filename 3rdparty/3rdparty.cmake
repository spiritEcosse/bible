find_package(Git)

#set(REBUILD_CURL On)
#set(REBUILD_NGHTTP2 On)
option(USE_BUNDLED_DEPS "Enable bundled dependencies instead of using the system ones" ON)
option(REBUILD_OPENSSL OFF)
option(REBUILD_CURL OFF)
option(REBUILD_NGHTTP2 OFF)
set(LIB_NGHTTP2 nghttp2)
set(LIB_CURL curl)
set(LIB_OPENSSL openssl)
set(LIB_SQLITE_ORM sqlite_orm)
set(LIB_QUAZIP quazip)
set(LIB_LCOV lcov)
include(ProcessorCount)
ProcessorCount(CORES)
set(OPENSSL_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_OPENSSL}")
set(QUAZIP_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_QUAZIP}")
set(SQLITE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_SQLITE_ORM}")


if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    message(STATUS "========================Submodule update========================")
    execute_process(COMMAND bash -c "curl https://raw.githubusercontent.com/spiritEcosse/aws-sailfish-sdk/master/install.sh | bash -s -- --func=git_submodule_checkout"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT)

    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with : ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()

    include(${LIB_QUAZIP})
    include(${LIB_SQLITE_ORM})
    include(${LIB_CURL})
endif()


if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    include(${LIB_LCOV})
endif()

