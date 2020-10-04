project(dbmanager LANGUAGES CXX)

include_directories(src/include)

set(RESOURCES
  dbmanager_global.h
  src/include/Executor.h
  src/include/Selector.h
  src/include/dbmapper.h
  src/include/ConnectionManager.h
  src/ConnectionManager.cpp
  src/processor.cpp
  src/Executor.cpp
  src/Selector.cpp
  src/include/manipulator.h
  src/manipulator.cpp
  include/processor.h
  include/dbtypes.h
)

add_library(${PROJECT_NAME} STATIC ${RESOURCES})

find_package(Qt5 COMPONENTS Core Sql REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Sql)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        ${CPP_REDIS_INCLUDE_DIR}
        ${TACOPIE_INCLUDE_DIR}
        src
)

target_link_libraries(${PROJECT_NAME} PRIVATE modules)
target_link_libraries(${PROJECT_NAME} PRIVATE cpp_redis)
target_compile_definitions(${PROJECT_NAME} PRIVATE DBMANAGER_LIBRARY)
