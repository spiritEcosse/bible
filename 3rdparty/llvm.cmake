message(STATUS "========================Start configure ${LIB_LLVM}====================")

set(LLVM_BUNDLE_DIR "${PROJECT_BINARY_DIR}/3rdparty/${LIB_LLVM}")
set(LLVM_INSTALL_DIR "${LLVM_BUNDLE_DIR}/build")
set(LLVM_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/${LIB_LLVM}")

if(NOT EXISTS ${LLVM_INSTALL_DIR})
	# TODO: add : checkout to some version, not just last
	execute_process(COMMAND bash -c "git clone --depth 1 https://github.com/llvm/llvm-project.git ${LIB_LLVM}"
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/3rdparty)
#endif()

	execute_process(COMMAND bash -c "\
		cmake -S llvm \
		-B build \
		-DLLVM_ENABLE_PROJECTS='clang;clang-tools-extra;compiler-rt;llvm' \
		-DLLVM_TARGETS_TO_BUILD='ARM;X86;AArch64' \
		-DLLVM_ENABLE_ASSERTIONS=ON \
		-DCMAKE_BUILD_TYPE=Release"
			WORKING_DIRECTORY ${LLVM_BUNDLE_DIR} RESULT_VARIABLE OUTPUT_BUILD_LLVM)
	execute_process(COMMAND bash -c "ninja -C build check-all"
			WORKING_DIRECTORY ${LLVM_BUNDLE_DIR}
			RESULT_VARIABLE LLVM_BUILD_RESULT)
	message("OUTPUT_BUILD_LLVM: ${OUTPUT_BUILD_LLVM}")
	if(NOT LLVM_BUILD_RESULT EQUAL "0")
		message(FATAL_ERROR "cmake build llvm is failed")
	endif()
endif()

set(ENV{PATH} "${LLVM_INSTALL_DIR}/bin:$ENV{PATH}")

message(STATUS "========================End configure ${LIB_LLVM}====================")
