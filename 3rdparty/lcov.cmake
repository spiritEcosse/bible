message(STATUS "========================Start configure ${LIB_LCOV}====================")

set(LCOV_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_LCOV}")
set(LCOV_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_LCOV}")

if(NOT EXISTS "${LCOV_SRC_DIR}")
	execute_process(COMMAND bash -c "git clone --depth 1 https://github.com/linux-test-project/lcov.git ${LIB_LCOV}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
endif()

set(ENV{PATH} "${LCOV_BUNDLE_DIR}/bin:$ENV{PATH}")

message(STATUS "========================End configure ${LIB_LCOV}====================")
