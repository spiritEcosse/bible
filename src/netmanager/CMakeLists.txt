project(netmanager LANGUAGES CXX)

include_directories(include)
include_directories(${CURL_INCLUDE_DIR})

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

find_package(Qt5 COMPONENTS Core Network Qml Sql REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt5::Core Qt5::Network Qt5::Qml Qt5::Sql modules core dbmanager ssl crypto curl ZLIB::ZLIB nghttp2 dl)

#if (NOT ${SAILFISH})
#    target_link_libraries(${PROJECT_NAME} INTERFACE openssl)
#endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
        include
    PRIVATE
        src
)

target_compile_definitions(${PROJECT_NAME} PRIVATE NETMANAGER_LIBRARY)
