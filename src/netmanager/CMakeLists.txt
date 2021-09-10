project(netmanager LANGUAGES CXX)

include_directories(include)

add_library(${PROJECT_NAME} STATIC
  # headers
  include/netmanager_global.h
  include/downloadmanager.h
  include/textprogressbar.h
  include/quickdownload.h
  include/curlmulti.h
  include/curleasy.h
  # sources
  src/textprogressbar.cpp
  src/quickdownload.cpp
  src/downloadmanager.cpp
  src/curlmulti.cpp
  src/curleasy.cpp
)

find_package(Qt5 COMPONENTS Core Network Qml REQUIRED)
find_package(CURL)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Network Qt5::Qml sqlite_orm CURL)

if (NOT ${SAILFISH})
    target_link_libraries(${PROJECT_NAME} PRIVATE ssl crypto)
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
        ${SQLITE_ORM_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE modules core dbmanager)
target_compile_definitions(${PROJECT_NAME} PRIVATE NETMANAGER_LIBRARY)
