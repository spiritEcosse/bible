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

        if (NOT ${SAILFISH})
            set(QUAZIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/quazip/quazip
                    CACHE PATH "quazip include directory")
            add_subdirectory(3rdparty/quazip)
            add_subdirectory(3rdparty/openssl)
        endif()
        execute_process(COMMAND bash -c "sed -e '/add_subdirectory(examples)/ s/^#*/#/' -i 3rdparty/sqlite_orm/CMakeLists.txt"
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}) # remove sinse with sqlite_orm==1.6
        add_subdirectory(3rdparty/sqlite_orm)
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/3rdparty/quazip/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
