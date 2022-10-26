message(STATUS "========================Start configure ${LIB_LLVM}====================")

set(LLVM_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_LLVM}")
set(LLVM_INSTALL_DIR "${LLVM_BUNDLE_DIR}/build")

if(NOT EXISTS "${LLVM_BUNDLE_DIR}")
	execute_process(COMMAND bash -c "git clone --depth 1 https://github.com/llvm/llvm-project.git ${LIB_LLVM}"
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/3rdparty)
endif()

if(NOT EXISTS ${LLVM_INSTALL_DIR})
    execute_process(COMMAND bash -c "cmake -S llvm -B build -DLLVM_CODE_COVERAGE_TARGETS=llvm-cov -DCMAKE_BUILD_TYPE=Release -G '${CMAKE_GENERATOR}'"
            WORKING_DIRECTORY ${LLVM_BUNDLE_DIR}
            RESULT_VARIABLE OUTPUT_CONF_LLVM)
    if(NOT OUTPUT_CONF_LLVM EQUAL "0")
        message(FATAL_ERROR "cmake -S llvm -B build ...: ${OUTPUT_CONF_LLVM}")
    endif()
    execute_process(COMMAND bash -c "cmake --build build --target llvm-cov"
            WORKING_DIRECTORY ${LLVM_BUNDLE_DIR} RESULT_VARIABLE OUTPUT_BUILD_LLVM)
    if(NOT OUTPUT_BUILD_LLVM EQUAL "0")
        message(FATAL_ERROR "cmake --build build ...: ${OUTPUT_BUILD_LLVM}")
    endif()
endif()

set(ENV{PATH} "${LLVM_INSTALL_DIR}/bin:$ENV{PATH}")

message(STATUS "========================End configure ${LIB_LLVM}====================")
