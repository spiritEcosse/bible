find_package(Git QUIET)

option(BUILD_EXAMPLES OFF) # option BUILD_EXAMPLES available sinse with sqlite_orm==1.6
option(SqliteOrm_BuildTests OFF)

execute_process(COMMAND ${GIT_EXECUTABLE} submodule
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_SUBMODULE)
message(${CMAKE_CURRENT_SOURCE_DIR})
message(${PROJECT_SOURCE_DIR})
execute_process(COMMAND ls -la)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with : ${GIT_SUBMOD_RESULT}, please checkout submodules")
        else()
            set(QUAZIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/quazip/quazip
                    CACHE PATH "quazip include directory")
            set(SQLITE_ORM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/sqlite_orm/include/sqlite_orm
                    CACHE PATH "sqlite_orm include directory")
            add_subdirectory(3rdparty/quazip)
            add_subdirectory(3rdparty/sqlite_orm)
            add_subdirectory(3rdparty/openssl)
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/3rdparty/quazip/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
