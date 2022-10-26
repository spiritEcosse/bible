message(STATUS "========================Start configure ${LIB_LLVM}====================")

set(LLVM_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_LLVM}")
set(LLVM_INSTALL_DIR "${LLVM_BUNDLE_DIR}/build")
set(LLVM_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_LLVM}")

if(NOT EXISTS "${LLVM_SRC_DIR}")
	execute_process(COMMAND bash -c "git clone --depth 1 https://github.com/llvm/llvm-project.git ${LIB_LLVM}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
endif()

execute_process(COMMAND bash -c "ls -la ${LLVM_INSTALL_DIR}"
        WORKING_DIRECTORY ${LLVM_BUNDLE_DIR}
        RESULT_VARIABLE LS_LA_LLVM_INSTALL_DIR)
message("LS_LA_LLVM_INSTALL_DIR: ${LS_LA_LLVM_INSTALL_DIR}")

if(NOT EXISTS ${LLVM_INSTALL_DIR})
    execute_process(COMMAND bash -c "cmake -S llvm -DCMAKE_BUILD_TYPE=Release -G '${CMAKE_GENERATOR}'"
            WORKING_DIRECTORY ${LLVM_BUNDLE_DIR}
            RESULT_VARIABLE OUTPUT_CONF_LLVM)
    if(NOT OUTPUT_CONF_LLVM EQUAL "0")
        message(FATAL_ERROR "cmake -S llvm : ${OUTPUT_CONF_LLVM}")
    endif()
    execute_process(COMMAND bash -c "cmake --build . --target llvm-cov"
            WORKING_DIRECTORY ${LLVM_INSTALL_DIR} RESULT_VARIABLE OUTPUT_BUILD_LLVM)
    if(NOT OUTPUT_BUILD_LLVM EQUAL "0")
        message(FATAL_ERROR "cmake --build build: ${OUTPUT_BUILD_LLVM}")
    endif()
endif()

set(ENV{PATH} "${LLVM_INSTALL_DIR}/bin:$ENV{PATH}")

message(STATUS "========================End configure ${LIB_LLVM}====================")
