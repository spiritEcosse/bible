project(netmanager LANGUAGES CXX)

include_directories(include)

add_library(${PROJECT_NAME} STATIC
  include/netmanager_global.h
  include/downloadmanager.h
  include/textprogressbar.h
  src/textprogressbar.cpp
  src/downloadmanager.cpp
)

find_package(Qt5 COMPONENTS Core Network REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Network)

if (NOT ${SAILFISH})
    target_link_libraries(${PROJECT_NAME} PRIVATE ssl crypto)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
)

target_compile_definitions(${PROJECT_NAME} PRIVATE NETMANAGER_LIBRARY)
