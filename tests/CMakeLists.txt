cmake_minimum_required(VERSION 3.1.0)
project(tests)

# run moc automatically
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

# GTest
enable_testing()

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download"
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This adds the following targets:
# gtest, gtest_main, gmock and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src"
                 "${CMAKE_BINARY_DIR}/googletest-build"
)

# The gtest/gmock targets carry header search path dependencies
# automatically when using CMake 2.8.11 or later. Otherwise we
# have to add them here ourselves.
if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include"
    )
endif()

set(SRC_PATH ../../src)

set(Headers
    mock_downloadmanager.h
    mock_modulesmodel.h
    mock_modulesgroupmodel.h
    mock_downloadmanager.h
    mock_qfile.h
    mock_qfileinfo.h
    mock_qnetworkreply.h
    ${SRC_PATH}/DownloadManager.h
    ${SRC_PATH}/TextProgressBar.h
    ${SRC_PATH}/ModulesModel.h
    ${SRC_PATH}/ModulesGroupModel.h
    QtSql/QSqlDatabase
    QtSql/qsqldatabase.h
    QtSql/QSqlError
    QtSql/qsqlerror.h
    QtSql/QSqlQuery
    QtSql/qsqlquery.h
    QtSql/QSqlRecord
    QtSql/qsqlrecord.h
    QtSql/qsqlquerymodel.h
    QtSql/QSqlQueryModel
    QtSql/QSqlTableModel
    QtSql/qsqltablemodel.h
    QtNetwork/qnetworkrequest.h
    QtCore/QQueue
    QtCore/QtCore
    QtCore/qqueue.h
    QtCore/qurl.h
    QtCore/QUrl
    QtCore/QFile
    QtCore/qfile.h
    QtCore/QVariant
    QtCore/qvariant.h
    QtCore/QFileInfo
    QtCore/qfileinfo.h
    QtNetwork/QNetworkReply
    QtNetwork/qnetworkreply.h
    QtNetwork/qsslerror.h
    QtNetwork/QSslError
    QtCore/QStringList
    QtCore/qstringlist.h
    quazip/quazip.h
    quazip/JlCompress.h
    mock_qstringlist.h
    mock_qtimer.h
    mock_qnetworkrequest.h
    mock_qsqlquery.h
    mock_qsqldatabase.h
    mock_qsqlrecord.h
    mock_qsqlerror.h
    gtest_global.h
    mock_qqueue.h
    mock_qurl.h
    mock_qvariant.h
    mock_textprogressbar.h
    mock_qsqltablemodel.h
)

set(Sources
    modulesgroupmodel-test.cpp
    modulesmodel-test.cpp
    textprogressbar-test.cpp
    main.cpp
    downloadmanager-test.cpp
    ${SRC_PATH}/DownloadManager.cpp
    ${SRC_PATH}/TextProgressBar.cpp
    ${SRC_PATH}/dbmanager.cpp
    ${SRC_PATH}/ModulesModel.cpp
    ${SRC_PATH}/ModulesGroupModel.cpp
)

include_directories(
    QtCore
    QtNetwork
    QtSql
    quazip
)

add_executable(${CMAKE_PROJECT_NAME} ${Sources} ${Headers})

find_package(Qt5Core REQUIRED)

target_link_libraries(
    ${CMAKE_PROJECT_NAME}
    gtest
    gmock
    Qt5::Core
#    remove quazip5
    quazip5
)

find_package(Qt5 COMPONENTS Core REQUIRED)
get_target_property(QtCore_location Qt5::Core LOCATION)
