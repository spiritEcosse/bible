find_package(Git QUIET)

option(BUILD_EXAMPLES OFF) # option BUILD_EXAMPLES available sinse with sqlite_orm==1.6
option(SqliteOrm_BuildTests OFF)

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    message(STATUS "========================Submodule update========================")
    execute_process(COMMAND bash -c "git submodule foreach -q --recursive 'git checkout $(git config -f $toplevel/.gitmodules submodule.$name.branch || echo master)'"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init failed with : ${GIT_SUBMOD_RESULT}, please checkout submodules")
    else()
        set(SQLITE_ORM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/sqlite_orm/include/sqlite_orm
                CACHE PATH "sqlite_orm include directory")
        set(CURLPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/curlpp/include/
                CACHE PATH "curlpp include directory")

        if (NOT ${SAILFISH})
            set(QUAZIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/quazip/quazip
                    CACHE PATH "quazip include directory")
            execute_process(COMMAND bash -c "sed -i '' -e '/add_subdirectory(qztest EXCLUDE_FROM_ALL)/ s/^#*/#/' 3rdparty/quazip/CMakeLists.txt"
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
            add_subdirectory(3rdparty/quazip)
            add_subdirectory(3rdparty/openssl)
        endif()
        add_subdirectory(3rdparty/sqlite_orm)
        add_subdirectory(3rdparty/curlpp)
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/3rdparty/quazip/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
