message(STATUS "========================Sart configure ${LIBTOOL}==========================")

set(LIBTOOL_VERSION 2.4)
set(LIBTOOL_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIBTOOL}")
set(LIBTOOL_INSTALL_DIR "${LIBTOOL_BUNDLE_DIR}/build")
set(LIBTOOL_BIN_DIR "${LIBTOOL_INSTALL_DIR}/bin")

if(NOT EXISTS ${LIBTOOL_INSTALL_DIR} OR ${REBUILD_LIBTOOL})
    execute_process(COMMAND bash -c "\
        cd ${PROJECT_BINARY_DIR} && \
        curl -OL http://ftpmirror.gnu.org/${LIBTOOL}/${LIBTOOL}-${LIBTOOL_VERSION}.tar.gz && \
        tar xzf ${LIBTOOL}-${LIBTOOL_VERSION}.tar.gz && \
        cd ${LIBTOOL}-${LIBTOOL_VERSION} && \
        ./configure --prefix=${LIBTOOL_INSTALL_DIR} && \
        make && \
        make install && \
        rm -fr ${LIBTOOL}-*")
endif()
message(STATUS "========================End configure ${LIBTOOL}==========================")
