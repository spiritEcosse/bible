message(STATUS "========================Start configure ${LIB_LLVM}====================")

set(LLVM_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_LLVM}")
set(LLVM_INSTALL_DIR "${LLVM_BUNDLE_DIR}/build")
set(LLVM_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_LLVM}")

if(NOT EXISTS ${LLVM_INSTALL_DIR})
	if (NOT ${LLVM_BUNDLE_DIR} STREQUAL ${LLVM_SRC_DIR})
		execute_process(COMMAND bash -c "\
			rm -fr ${LLVM_BUNDLE_DIR} && \
            cp -fr ${LLVM_SRC_DIR} ${LLVM_BUNDLE_DIR}")
	endif()

	execute_process(COMMAND bash -c "\
		cmake -S llvm \
		-B build \
		-DLLVM_ENABLE_PROJECTS='llvm-cov' \
		-DLLVM_TARGETS_TO_BUILD='ARM;X86;AArch64' \
		-DLLVM_ENABLE_ASSERTIONS=ON \
		-DCMAKE_BUILD_TYPE=Release"
			WORKING_DIRECTORY ${LLVM_BUNDLE_DIR} RESULT_VARIABLE OUTPUT_CONFIGURE_LLVM)
	if(NOT OUTPUT_CONFIGURE_LLVM EQUAL "0")
		message(FATAL_ERROR "cmake -S llvm ... OUTPUT_CONFIGURE_LLVM: ${OUTPUT_CONFIGURE_LLVM}")
	endif()

	execute_process(COMMAND bash -c "cmake --build build --target llvm-cov -j $(echo "2 * $(getconf _NPROCESSORS_ONLN)" | bc -l)"
			WORKING_DIRECTORY ${LLVM_BUNDLE_DIR}
			RESULT_VARIABLE LLVM_BUILD_RESULT)

	if(NOT LLVM_BUILD_RESULT EQUAL "0")
		message(FATAL_ERROR "cmake build llvm is failed. LLVM_BUILD_RESULT: ${LLVM_BUILD_RESULT}")
	endif()
endif()

set(ENV{PATH} "${LLVM_INSTALL_DIR}/bin:$ENV{PATH}")

message(STATUS "========================End configure ${LIB_LLVM}====================")
