message(STATUS "========================Start configure ${LIB_NGHTTP2}=====================")

option(USE_BUNDLED_NGHTTP2 "Enable building of the bundled ${LIB_NGHTTP2}" ${USE_BUNDLED_DEPS})

set(NGHTTP2_VERSION 1.44.0)
set(NGHTTP2_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_NGHTTP2}/")
set(NGHTTP2_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_NGHTTP2}")
set(NGHTTP2_INSTALL_DIR "${NGHTTP2_BUNDLE_DIR}/build")
set(NGHTTP2_INCLUDE_DIR "${NGHTTP2_INSTALL_DIR}/include")
set(NGHTTP2_LIB_DIR "${NGHTTP2_INSTALL_DIR}/lib")

if(NOT ${USE_BUNDLED_NGHTTP2})
    find_package(${LIB_NGHTTP2} REQUIRED)
    message(STATUS "Found ${LIB_NGHTTP2}: include: ${NGHTTP2_INCLUDE_DIR}, lib: ${NGHTTP2_LIBRARIES}")
elseif(EXISTS ${NGHTTP2_INCLUDE_DIR} AND EXISTS ${NGHTTP2_LIB_DIR} AND NOT ${REBUILD_NGHTTP2})
    message("we already have ${LIB_NGHTTP2}")
    option(STATUS_BUILD_NGHTTP2 "0")
else()
    if(NOT TARGET ${LIB_NGHTTP2})
        message(STATUS "Using bundled ${LIB_NGHTTP2} in '${NGHTTP2_BUNDLE_DIR}'")
        SET(REBUILD_CURL ON)

        execute_process(COMMAND bash -c "\
            cd ${PROJECT_BINARY_DIR} && \
            rm -fr ${NGHTTP2_INSTALL_DIR} && \
            curl -LOs https://github.com/${LIB_NGHTTP2}/${LIB_NGHTTP2}/releases/download/v${NGHTTP2_VERSION}/${LIB_NGHTTP2}-${NGHTTP2_VERSION}.tar.gz && \
            tar xzf ${LIB_NGHTTP2}-${NGHTTP2_VERSION}.tar.gz && \
            cd ${LIB_NGHTTP2}-${NGHTTP2_VERSION} && \
            export CFLAGS='-fPIC' && \
            ./configure \
                --prefix=${NGHTTP2_INSTALL_DIR} \
                --disable-shared \
                --disable-app \
                --disable-threads \
                --enable-lib-only \
            && make && \
            make install"
            RESULT_VARIABLE STATUS_BUILD_NGHTTP2)
        execute_process(COMMAND bash -c "\
            rm -fr ${LIB_NGHTTP2}-*"
        )

        if (NOT STATUS_BUILD_NGHTTP2 EQUAL "0")
            message(FATAL_ERROR "STATUS_BUILD_OPENSSL : ${STATUS_BUILD_OPENSSL}")
        endif()
    endif()
endif()

link_directories("${NGHTTP2_LIB_DIR}")
message(STATUS "========================End configure ${LIB_NGHTTP2}=======================")
