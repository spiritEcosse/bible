project(modules LANGUAGES CXX)

set(RESOURCES
    # models
    include/modelmodule.h
    include/modelgroupmodules.h
    include/modelregistry.h
    include/modelupdate.h
    src/modelmodule.cpp
    src/modelgroupmodules.cpp
    src/modelregistry.cpp
    src/modelupdate.cpp

    #entities
    include/registry.h
    include/module.h
    include/groupmodules.h
    src/registry.cpp
    src/module.cpp
    src/groupmodules.cpp

    #managers
    include/managergroup.h
    include/managerregistry.h
    src/managergroup.cpp
    src/managerregistry.cpp
)

add_library(${PROJECT_NAME} STATIC ${RESOURCES})

find_package(Qt5 COMPONENTS Core Sql Network REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Sql Qt5::Network QuaZip sqlite_orm)

find_package (SQLite3)
target_link_libraries (${PROJECT_NAME} PRIVATE SQLite::SQLite3)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
        ${SQLITE_ORM_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE core dbmanager netmanager)
target_compile_definitions(${PROJECT_NAME} PRIVATE MODULES_LIBRARY)
