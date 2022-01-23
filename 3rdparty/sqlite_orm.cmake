message(STATUS "========================Start configure ${LIB_SQLITE_ORM}====================")

option(BUILD_EXAMPLES OFF)
option(SqliteOrm_BuildTests OFF)

add_subdirectory(3rdparty/${LIB_SQLITE_ORM})
set(SQLITE_ORM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_SQLITE_ORM}/include/${LIB_SQLITE_ORM}
        CACHE PATH "sqlite_orm include directory")
include_directories(${SQLITE_ORM_INCLUDE_DIR})

message(STATUS "========================End configure ${LIB_SQLITE_ORM}====================")
