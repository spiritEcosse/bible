message(STATUS "========================Sart configure ${LIB_AUTOMAKE}==========================")

set(AUTOMAKE_VERSION 1.16.4)
set(AUTOMAKE_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_AUTOMAKE}")
set(AUTOMAKE_INSTALL_DIR "${AUTOMAKE_BUNDLE_DIR}/build")
set(AUTOMAKE_BIN_DIR "${AUTOMAKE_INSTALL_DIR}/bin")
set(AUTOMAKE_AUTOMAKE "${AUTOMAKE_BIN_DIR}/${LIB_AUTOMAKE}")

if(NOT EXISTS ${AUTOMAKE_INSTALL_DIR} OR ${REBUILD_AUTOMAKE})
    execute_process(COMMAND bash -c "\
        cd ${PROJECT_BINARY_DIR} && \
        curl -OL http://ftpmirror.gnu.org/${LIB_AUTOMAKE}/${LIB_AUTOMAKE}-${AUTOMAKE_VERSION}.tar.gz && \
        tar xzf ${LIB_AUTOMAKE}-${AUTOMAKE_VERSION}.tar.gz && \
        cd ${LIB_AUTOMAKE}-${AUTOMAKE_VERSION} && \
        export PATH=$PATH:${AUTOCONF_BIN_DIR} && \
        ./configure --prefix=${AUTOMAKE_INSTALL_DIR} && \
        make && \
        make install && \
        rm -fr ${LIB_AUTOMAKE}-*")
endif()

message(STATUS "========================End configure ${LIB_AUTOMAKE}==========================")
