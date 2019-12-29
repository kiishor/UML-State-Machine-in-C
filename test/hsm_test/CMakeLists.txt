cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project("hsm_UnitTest")

# Setup path for testcase dir
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(TESTCASE_DIR ${SRC_DIR}/case )
set(TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

set(TESTCASE_FILES
    ${TESTCASE_DIR}/simple_test.cpp
    ${TESTCASE_DIR}/priority_test.cpp
    ${TESTCASE_DIR}/state_transition.cpp
	${TESTCASE_DIR}/hierarchical_test.cpp
	${TESTCASE_DIR}/hierarchical_state_transition.cpp
)

set(TARGET_FILES 
	${TARGET_DIR}/hsm.c
	)

set (TEST_FILES 
	${SRC_DIR}/main.cpp)

set (HEADER_FILES
		${SRC_DIR}/catch.hpp
		${SRC_DIR}/hippomocks.h
		${TARGET_DIR}/hsm.h
	)
SOURCE_GROUP("Tests" FILES ${TEST_FILES} ${TARGET_FILES} ${TESTCASE_FILES} ${HEADER_FILES})

include(CTest)

include_directories(
						${SRC_DIR} 
						${TARGET_DIR}
					)


set(CPP_VERSION 11)
if ("cxx_std_14" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
	set(CPP_VERSION 14)
endif()

message("Your compiler supports : cpp${CPP_VERSION}")
set(CMAKE_CXX_STANDARD ${CPP_VERSION})

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(C_VERSION 99)
if ("c_std_11" IN_LIST CMAKE_C_COMPILE_FEATURES)
	set(C_VERSION 11)
endif()

set(CMAKE_C_STANDARD ${C_VERSION})
set(CMAKE_C_STANDARD_REQUIRED ON)
message("Your compiler supports : c${C_VERSION}")

set(HIERARCHICAL_STATES 1)
SET(COVERAGE OFF CACHE BOOL "Coverage")

add_executable(hsm_UnitTest ${TEST_FILES} ${TARGET_FILES} ${TESTCASE_FILES} ${HEADER_FILES})
add_test(hsm_UnitTest hsm_UnitTest)


if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU" )
    target_compile_options( hsm_UnitTest PRIVATE -Wall -Wextra -Wunreachable-code -Wpedantic)
    target_compile_options( hsm_UnitTest PRIVATE -Werror )
	set(HSM_USE_VARIABLE_LENGTH_ARRAY 1)
    if (COVERAGE)
        target_compile_options(hsm_UnitTest PRIVATE --coverage)
        target_link_libraries(hsm_UnitTest PRIVATE --coverage)
    endif()
endif()

# Clang specific options go here
if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang" )
    target_compile_options( hsm_UnitTest PRIVATE -Wweak-vtables -Wexit-time-destructors -Wglobal-constructors -Wmissing-noreturn )
endif()

if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    STRING(REGEX REPLACE "/W[0-9]" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) # override default warning level
    target_compile_options( hsm_UnitTest PRIVATE /w44265 /w44061 /w44062 /w45038 )
    target_compile_options( hsm_UnitTest PRIVATE /WX)
	set(HSM_USE_VARIABLE_LENGTH_ARRAY 0)
	set(MAX_HIERARCHICAL_LEVEL 3)
endif()

target_compile_definitions(hsm_UnitTest PRIVATE HSM_CONFIG)
configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/hsm_config.h.in"
            "${CMAKE_CURRENT_BINARY_DIR}/hsm_config.h" )
			

# Setup compiler include path
target_include_directories(hsm_UnitTest PRIVATE ${CMAKE_CURRENT_BINARY_DIR})


