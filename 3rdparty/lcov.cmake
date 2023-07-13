message(STATUS "========================Start configure ${LIB_LCOV}====================")

set(LCOV_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_LCOV}")
set(LCOV_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_LCOV}")

set(ENV{PATH} "${LCOV_SRC_DIR}/bin:$ENV{PATH}")

message(STATUS "========================End configure ${LIB_LCOV}====================")
