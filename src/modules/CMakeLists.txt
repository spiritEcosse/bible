project(modules LANGUAGES CXX)

set(RESOURCES
    # models
    include/modelmodule.h
    include/modelgroupmodules.h
    include/modelregistry.h
    include/modelupdate.h
    include/modelhost.h
    include/model.h
    src/model.cpp
    src/modelmodule.cpp
    src/modelgroupmodules.cpp
    src/modelregistry.cpp
    src/modelupdate.cpp
    src/modelhost.cpp

    #entities
    include/registry.h
    include/module.h
    include/groupmodules.h
    include/host.h
    src/registry.cpp
    src/host.cpp
    src/module.cpp
    src/groupmodules.cpp

    #managers
    include/managergroup.h
    include/managerregistry.h
    src/managergroup.cpp
    src/managerregistry.cpp
)

add_library(${PROJECT_NAME} STATIC ${RESOURCES})
#target_code_coverage(${PROJECT_NAME} AUTO ALL) # As an executable target, adds to the 'ccov' and ccov-all' targets, and the reports will exclude the non-covered.cpp file, and any files in a test/ folder.

find_package(Qt5 COMPONENTS Core Network REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Quick Qt5::Network sqlite_orm)

message("------------------------------------------------------------------------------Quazip---------------------------------------------------------")
if (${SAILFISH})
    find_package(QuaZip5)
    include_directories(${QUAZIP_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${QUAZIP_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE QuaZip)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
        ${SQLITE_ORM_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE core dbmanager netmanager)
target_compile_definitions(${PROJECT_NAME} PRIVATE MODULES_LIBRARY)
