message(STATUS "========================Sart configure ${LIB_AUTOCONF}==========================")

set(AUTOCONF_VERSION 2.71)
set(AUTOCONF_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_AUTOCONF}")
set(AUTOCONF_INSTALL_DIR "${AUTOCONF_BUNDLE_DIR}/build")
set(AUTOCONF_BIN_DIR "${AUTOCONF_INSTALL_DIR}/bin")
set(AUTOCONF_AUTORECONF "${AUTOCONF_INSTALL_DIR}/bin/autoreconf")
set(AUTOCONF_AUTOCONF "${AUTOCONF_INSTALL_DIR}/bin/autoconf")

if(NOT EXISTS ${AUTOCONF_INSTALL_DIR} OR ${REBUILD_AUTOCONF})
    execute_process(COMMAND bash -c "\
        cd ${PROJECT_BINARY_DIR} && \
        curl -OL http://ftpmirror.gnu.org/${LIB_AUTOCONF}/${LIB_AUTOCONF}-${AUTOCONF_VERSION}.tar.gz && \
        tar xzf ${LIB_AUTOCONF}-${AUTOCONF_VERSION}.tar.gz && \
        cd ${LIB_AUTOCONF}-${AUTOCONF_VERSION} && \
        ./configure --prefix=${AUTOCONF_INSTALL_DIR} && \
        make && \
        make install && \
        rm -fr ${LIB_AUTOCONF}-*")
endif()

message(STATUS "========================End configure ${LIB_AUTOCONF}==========================")
