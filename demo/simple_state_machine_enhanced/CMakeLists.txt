cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project("simple_state_machine_enhanced")

# Setup path for source dir
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

set(TARGET_FILES 
	${TARGET_DIR}/hsm.c
	)

set (DEMO_FILES 
	${SRC_DIR}/main.c
	${SRC_DIR}/demo_process.c
	)

set (HEADER_FILES
		${SRC_DIR}/demo_process.h
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

set(HIERARCHICAL_STATES 0)
set(STATE_MACHINE_LOGGER 1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(simple_state_machine_enhanced ${DEMO_FILES} ${TARGET_FILES} ${HEADER_FILES})

if ( CMAKE_C_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    target_compile_options( simple_state_machine_enhanced PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic)
    target_compile_options( simple_state_machine_enhanced PRIVATE -Werror )
endif()

target_compile_definitions(simple_state_machine_enhanced PRIVATE HSM_CONFIG)
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/hsm_config.h.in"
            "${CMAKE_CURRENT_BINARY_DIR}/hsm_config.h" )

# Setup compiler include path
target_include_directories(simple_state_machine_enhanced PRIVATE ${CMAKE_CURRENT_BINARY_DIR})


