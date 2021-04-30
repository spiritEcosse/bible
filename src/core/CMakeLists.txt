project(core LANGUAGES CXX)

include_directories(include)

set(RESOURCES
    include/global.h
    include/locallanguage.h
    include/invaliddata.h
    include/dereferenceiterator.h
    src/locallanguage.cpp
    )

add_library(${PROJECT_NAME} STATIC ${RESOURCES})

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Network)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
)

target_compile_definitions(${PROJECT_NAME} PRIVATE CORE_LIBRARY)
