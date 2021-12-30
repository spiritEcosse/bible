message(STATUS "========================Start configure ${LIB_CURL}========================")

option(USE_BUNDLED_CURL "Enable building of the bundled ${LIB_CURL}" ${USE_BUNDLED_DEPS})

set(CURL_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_CURL}/")
set(CURL_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_CURL}")
set(CURL_INSTALL_DIR "${CURL_BUNDLE_DIR}/build")
set(CURL_INCLUDE_DIR "${CURL_INSTALL_DIR}/include")
set(CURL_LIB_DIR "${CURL_INSTALL_DIR}/lib")
set(CURL_VERSION 7.74.0)

include(${LIB_OPENSSL})
include(${LIB_NGHTTP2})

if(NOT ${USE_BUNDLED_CURL})
    find_package(CURL REQUIRED)
    message(STATUS "Found CURL: include: ${CURL_INCLUDE_DIR}, lib: ${CURL_LIBRARIES}")
elseif(EXISTS ${CURL_INCLUDE_DIR} AND EXISTS ${CURL_LIB_DIR} AND NOT ${REBUILD_CURL})
    message("we already have ${LIB_CURL}")
else()
    if(NOT TARGET ${LIB_CURL})        
        message(STATUS "Using bundled ${LIB_CURL} in '${CURL_BUNDLE_DIR}'")

        if(NOT ${USE_BUNDLED_OPENSSL})
            set(CURL_SSL_OPTION "--with-ssl")
        else()
            set(CURL_SSL_OPTION "--with-ssl=${OPENSSL_INSTALL_DIR}")
            message(STATUS "Using ${LIB_OPENSSL} for ${LIB_CURL} in '${CURL_SSL_OPTION}'")
        endif()

        if(NOT ${USE_BUNDLED_NGHTTP2})
            set(CURL_NGHTTP2_OPTION "--with-nghttp2")
        else()
            set(CURL_NGHTTP2_OPTION "--with-nghttp2=${NGHTTP2_INSTALL_DIR}")
            message(STATUS "Using ${LIB_NGHTTP2} for ${LIB_CURL} in '${CURL_NGHTTP2_OPTION}'")
        endif()

#        set(ARCH x86_64)
#        set(SDK macosx)
#        set(HOST "${ARCH}-apple-darwin")
#        set(DEPLOYMENT_TARGET 11.1)
#        execute_process (
#            COMMAND bash -c "xcrun -sdk ${SDK} --show-sdk-path"
#            OUTPUT_VARIABLE SYSROOT_SDK
#        )
#        set(CFLAGS "-arch ${ARCH} -isysroot ${SYSROOT_SDK} -m${SDK}-version-min=${DEPLOYMENT_TARGET}")
#        set(CFLAGS "-arch ${ARCH} -pipe -Os -gdwarf-2 -fembed-bitcode -m${SDK}-version-min=${DEPLOYMENT_TARGET}")
        execute_process(COMMAND bash -c "\
            cd ${PROJECT_BINARY_DIR} && \
            rm -fr ${CURL_INSTALL_DIR} && \
            export PKG_CONFIG_PATH=${NGHTTP2_INSTALL_DIR}/lib/pkgconfig:$PKG_CONFIG_PATH && \
            curl -LOs https://curl.haxx.se/download/${LIB_CURL}-${CURL_VERSION}.tar.gz && \
            tar xzf ${LIB_CURL}-${CURL_VERSION}.tar.gz && \
            cd ${LIB_CURL}-${CURL_VERSION} && \
            ./configure \
                --host=${TARGET_CURL} \
                --prefix=${CURL_INSTALL_DIR} \
                --enable-static \
                --with-random=/dev/urandom \
                ${CURL_SSL_OPTION} \
                ${CURL_NGHTTP2_OPTION} \
                --disable-shared \
                --with-pic \
                --enable-optimize \
                --disable-curldebug \
                --disable-rt \
                --enable-http \
                --disable-ftp \
                --enable-file \
                --disable-ldap \
                --disable-ldaps \
                --disable-rtsp \
                --disable-telnet \
                --disable-tftp \
                --disable-pop3 \
                --disable-imap \
                --disable-smb \
                --disable-smtp \
                --disable-gopher \
                --disable-sspi \
                --disable-ntlm-wb \
                --disable-tls-srp \
                --without-winssl \
                --without-darwinssl \
                --without-polarssl \
                --without-cyassl \
                --without-nss \
                --without-axtls \
                --without-ca-path \
                --without-ca-bundle \
                --without-libmetalink \
                --without-librtmp \
                --without-winidn \
                --without-libidn2 \
                --without-libpsl \
                --without-libssh2 \
                --disable-threaded-resolver \
                --without-brotli \
                --without-zstd \
                && make -j${CORES} \
                && make install"
                RESULT_VARIABLE STATUS_BUILD_CURL)

        if (NOT STATUS_BUILD_CURL EQUAL "0")
            message(FATAL_ERROR "STATUS_BUILD_CURL : ${STATUS_BUILD_CURL}")
        endif()
    endif()
endif()

link_directories("${CURL_LIB_DIR}")
include_directories("${CURL_INCLUDE_DIR}")

message(STATUS "========================End configure ${LIB_CURL}==========================")
