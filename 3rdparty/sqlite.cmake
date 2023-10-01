message(STATUS "========================Start configure ${LIB_SQLITE}====================")

set(SQLITE_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_SQLITE}")
set(SQLITE_INSTALL_DIR "${SQLITE_BUNDLE_DIR}/build")
set(SQLITE_INCLUDE_DIR "${SQLITE_INSTALL_DIR}/include")
set(SQLITE_LIB_DIR "${SQLITE_INSTALL_DIR}/lib")

message(${SQLITE_BUNDLE_DIR})
message(${SQLITE_SRC_DIR})

if(NOT EXISTS ${SQLITE_INSTALL_DIR})
	if (NOT ${SQLITE_BUNDLE_DIR} STREQUAL ${SQLITE_SRC_DIR})
		execute_process(COMMAND bash -c "\
			rm -fr ${SQLITE_BUNDLE_DIR} && \
            cp -fr ${SQLITE_SRC_DIR} ${SQLITE_BUNDLE_DIR}")
	endif()

	execute_process(COMMAND bash -c "\
		./configure --prefix=${SQLITE_INSTALL_DIR} && \
		make && \
        make install"
			WORKING_DIRECTORY ${SQLITE_BUNDLE_DIR} RESULT_VARIABLE STATUS_BUILD_SQLITE)
	if(NOT STATUS_BUILD_SQLITE EQUAL "0")
		message(FATAL_ERROR "./configure ... STATUS_BUILD_SQLITE: ${STATUS_BUILD_SQLITE}")
	endif()
endif()

link_directories("${SQLITE_LIB_DIR}")
include_directories("${SQLITE_INCLUDE_DIR}")

set(SQLite3_INCLUDE_DIR ${SQLITE_INCLUDE_DIR})
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(SQLite3_LIBRARY ${SQLITE_LIB_DIR}/libsqlite3.dylib)
else()
	set(SQLite3_LIBRARY ${SQLITE_LIB_DIR}/libsqlite3.so)
endif()

message(STATUS "========================End configure ${LIB_SQLITE}====================")
