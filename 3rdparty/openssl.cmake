message(STATUS "========================Start configure ${LIB_OPENSSL}=====================")

option(USE_BUNDLED_OPENSSL "Enable building of the bundled ${LIB_OPENSSL}" ${USE_BUNDLED_DEPS})

set(OPENSSL_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_OPENSSL}")
set(OPENSSL_INSTALL_DIR "${OPENSSL_BUNDLE_DIR}/build")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_INSTALL_DIR}/include")
set(OPENSSL_LIB_DIR "${OPENSSL_INSTALL_DIR}/lib")

if(NOT ${USE_BUNDLED_OPENSSL})
    find_package(OpenSSL REQUIRED)
    message(STATUS "Found ${LIB_OPENSSL}: include: ${OPENSSL_INCLUDE_DIR}, lib: ${OPENSSL_LIBRARIES}")
elseif(EXISTS ${OPENSSL_INCLUDE_DIR} AND EXISTS ${OPENSSL_LIB_DIR} AND NOT ${REBUILD_OPENSSL})
    message("we already have ${LIB_OPENSSL}")
    option(STATUS_BUILD_OPENSSL "0")
else()
    if(NOT TARGET_OPENSSL ${LIB_OPENSSL})
        message(STATUS "Using bundled ${LIB_OPENSSL} in '${OPENSSL_BUNDLE_DIR}'")

        SET(REBUILD_CURL ON)

        set(CUSTOMCONFIG "enable-ssl3 enable-ssl3-method enable-ssl-trace")

        message(${OPENSSL_BUNDLE_DIR})
        message(${OPENSSL_SRC_DIR})

        if (NOT ${OPENSSL_BUNDLE_DIR} STREQUAL ${OPENSSL_SRC_DIR})
            execute_process(COMMAND bash -c "\
                rm -fr ${OPENSSL_BUNDLE_DIR} && \
                cp -fr ${OPENSSL_SRC_DIR} ${OPENSSL_BUNDLE_DIR}")
        endif()

        if (${TARGET_OPENSSL} MATCHES "darwin64-arm64-cc")
            # as it explains in this link https://stackoverflow.com/questions/9952612/mac-os-usr-bin-env-bad-interpreter-operation-not-permitted
            execute_process(COMMAND bash -c "\
                xattr -d com.apple.quarantine Configure"
                WORKING_DIRECTORY ${OPENSSL_BUNDLE_DIR})
        endif()
        execute_process(COMMAND bash -c "\
            ./Configure no-asm ${TARGET_OPENSSL} no-shared no-tests --prefix=${OPENSSL_INSTALL_DIR} ${CUSTOMCONFIG} && \
            make -j${CORES} && \
            make install_sw"
            WORKING_DIRECTORY ${OPENSSL_BUNDLE_DIR}
            RESULT_VARIABLE STATUS_BUILD_OPENSSL)

        if (NOT STATUS_BUILD_OPENSSL EQUAL "0")
            message(FATAL_ERROR "STATUS_BUILD_OPENSSL : ${STATUS_BUILD_OPENSSL}")
        endif()
    endif()
endif()

link_directories("${OPENSSL_LIB_DIR}")
include_directories("${OPENSSL_INCLUDE_DIR}")

message(STATUS "========================End configure ${LIB_OPENSSL}=======================")
