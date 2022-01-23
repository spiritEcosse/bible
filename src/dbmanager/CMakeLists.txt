project(dbmanager LANGUAGES CXX)

set(RESOURCES
  include/db.h
  include/binding.h
  src/db.cpp
)

add_library(${PROJECT_NAME} STATIC ${RESOURCES})

find_package(Qt5 COMPONENTS Core Sql REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Sql sqlite_orm)

if ( ${SAILFISH} )
    include_directories(/usr/include)
    link_directories(/usr/lib)
    target_link_libraries(${PROJECT_NAME} PRIVATE sqlite3)
else()
    find_package (SQLite3)
    target_link_libraries (${PROJECT_NAME} PRIVATE SQLite::SQLite3)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
)

target_link_libraries(${PROJECT_NAME} PRIVATE modules core)
target_compile_definitions(${PROJECT_NAME} PRIVATE DBMANAGER_LIBRARY)
