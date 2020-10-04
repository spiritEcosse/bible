find_package(Git QUIET)

if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        else()
            set(QUAZIP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/quazip/quazip
                    CACHE PATH "quazip include directory")
            set(ACL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/acl/lib_acl_cpp/include
                    CACHE PATH "acl include directory")
            set(CPP_REDIS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cpp_redis/includes
                    CACHE PATH "cpp_redis include directory")
            set(TACOPIE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cpp_redis/tacopie/includes
                    CACHE PATH "tacopie include directory")
            add_subdirectory(3rdparty/quazip)
            add_subdirectory(3rdparty/cpp_redis)
#            add_subdirectory(3rdparty/hiredis)
#            add_subdirectory(3rdparty/acl)
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/3rdparty/quazip/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()
