cmake_minimum_required (VERSION 3.5)
project (UML_State_Machine LANGUAGES C VERSION 1.0 )

include(CTest)

# define some folders
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(DEMO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/demo)

add_subdirectory(test)
