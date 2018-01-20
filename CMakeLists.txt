cmake_minimum_required(VERSION 2.6)

project("kvfs" C)

enable_language(C)

if (CMAKE_COMPILER_IS_GNUCC)
	set (CMAKE_C_FLAGS "-Wall -Wextra -Werror -Wfatal-errors -std=c99")
endif (CMAKE_COMPILER_IS_GNUCC)

add_library("kvfs" "kvfs.c" "kvfs.h")

enable_testing()

add_executable("kvfs-test" "kvfs-test.c")
target_link_libraries("kvfs-test" "kvfs")
add_test("kvfs-test" "kvfs-test")
