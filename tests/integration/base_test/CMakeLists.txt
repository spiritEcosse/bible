project(base_test LANGUAGES CXX)

set(RESOURCES
    include/entitybase.h
    include/basetest.h
    include/modeljsontest.h
    src/basetest.cpp
    src/modeljsontest.cpp
    src/entitybase.cpp
)

add_library(${PROJECT_NAME} STATIC ${RESOURCES})
find_package(Qt5 COMPONENTS Core Test Network REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Test Qt5::Network sqlite_orm dbmanager netmanager core modules)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
        ${SQLITE_ORM_INCLUDE_DIR}
)
