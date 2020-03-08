cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project("hierarchical_state_machine")

# Setup path for source dir
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

set(TARGET_FILES
	${TARGET_DIR}/hsm.c
	)

set (DEMO_FILES
	${SRC_DIR}/main.c
	${SRC_DIR}/demo.c
	)

set (HEADER_FILES
		${SRC_DIR}/demo.h
		${TARGET_DIR}/hsm.h
	)
SOURCE_GROUP("Src" FILES ${DEMO_FILES} ${TARGET_FILES} ${HEADER_FILES})

include_directories(
						${SRC_DIR}
						${TARGET_DIR}
					)

set(C_VERSION 99)
if ("c_std_11" IN_LIST CMAKE_C_COMPILE_FEATURES)
	set(C_VERSION 11)
endif()

set(CMAKE_C_STANDARD ${C_VERSION})
set(CMAKE_C_STANDARD_REQUIRED ON)
message("Your compiler supports : c${C_VERSION}")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(hierarchical_state_machine ${DEMO_FILES} ${TARGET_FILES} ${HEADER_FILES})

if ( CMAKE_C_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    target_compile_options( hierarchical_state_machine PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic)
    target_compile_options( hierarchical_state_machine PRIVATE -Werror )
endif()

# Setup compiler include path
target_include_directories(hierarchical_state_machine PRIVATE ${CMAKE_CURRENT_BINARY_DIR})


