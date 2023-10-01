project(dbmanager LANGUAGES CXX)

set(RESOURCES include/db.h include/binding.h src/db.cpp)

add_library(${PROJECT_NAME} STATIC ${RESOURCES})

if(Qt6_FOUND)
  find_package(
    Qt6
    COMPONENTS Core
    REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core sqlite_orm)
else()
  find_package(
    Qt5
    COMPONENTS Core
    REQUIRED)
  target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core sqlite_orm)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE sqlite3)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC include
  PRIVATE src)

target_link_libraries(${PROJECT_NAME} PRIVATE modules core)
target_compile_definitions(${PROJECT_NAME} PRIVATE DBMANAGER_LIBRARY)
