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
    message("Doesn't exist: ${LLVM_INSTALL_DIR}")
    execute_process(COMMAND bash -c "cmake -S llvm -B build -DLLVM_CODE_COVERAGE_TARGETS=llvm-cov -DCMAKE_BUILD_TYPE=Release -G '${CMAKE_GENERATOR}' ../${LIB_LLVM}"
            WORKING_DIRECTORY ${LLVM_BUNDLE_DIR} RESULT_VARIABLE OUTPUT_BUILD_LLVM)
    message("OUTPUT_BUILD_LLVM: ${OUTPUT_BUILD_LLVM}")
    execute_process(COMMAND bash -c "cmake --build build --target llvm-cov"
            WORKING_DIRECTORY ${LLVM_BUNDLE_DIR} RESULT_VARIABLE OUTPUT_BUILD_LLVM1)
    message("OUTPUT_BUILD_LLVM1: ${OUTPUT_BUILD_LLVM1}")
endif()

set(ENV{PATH} "${LLVM_INSTALL_DIR}/bin:$ENV{PATH}")

message(STATUS "========================End configure ${LIB_LLVM}====================")
