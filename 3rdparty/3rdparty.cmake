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
set(LIB_LLVM llvm)
set(LIB_SQLITE sqlite)
include(ProcessorCount)
ProcessorCount(CORES)
set(LLVM_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_LLVM}")
set(SQLITE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_SQLITE}")
set(OPENSSL_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_OPENSSL}")
set(QUAZIP_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_QUAZIP}")

if (NOT EXISTS ${PROJECT_BINARY_DIR}/3rdparty)
    execute_process(COMMAND bash -c "mkdir ${PROJECT_BINARY_DIR}/3rdparty")
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    message(STATUS "========================Submodule update========================")
    execute_process(COMMAND bash -c "curl https://raw.githubusercontent.com/spiritEcosse/aws-sailfish-sdk/master/install.sh | bash -s -- --func=git_submodule_checkout"
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT)

    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git_submodule_checkout failed with : ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()

    include(${LIB_SQLITE})
    include(${LIB_QUAZIP})
    include(${LIB_SQLITE_ORM})
    include(${LIB_CURL})
endif()


if(CMAKE_C_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    include(${LIB_LCOV})
endif()

set(ENV{PATH} "/usr/local/bin/:$ENV{PATH}")

if (BUILD_LLVM OR (BUILD_TESTING AND CODE_COVERAGE))
    include(${LIB_LLVM})
endif()
