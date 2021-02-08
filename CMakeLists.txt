cmake_minimum_required(VERSION 3.17.0)
project(stopwatch VERSION 0.1.0)

cmake_dependent_option(STOPWATCH_BUILD_EXAMPLES
    "Build stopwatch examples" ON
    "CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME" OFF
)

# Main library =========================================================================================================
add_library(stopwatch INTERFACE)
add_library(stopwatch::stopwatch ALIAS stopwatch)

target_compile_features(stopwatch INTERFACE cxx_std_17)
target_sources(stopwatch INTERFACE
	$<BUILD_INTERFACE: src/stopwatch.hpp>
	$<INSTALL_INTERFACE: include/stopwatch.hpp>
)
target_include_directories(stopwatch INTERFACE
	$<BUILD_INTERFACE: src>
	$<INSTALL_INTERFACE: include>
)

install(TARGETS stopwatch)
install(DIRECTORY src/
	DESTINATION include
	COMPONENT Devel
)

# Examples =============================================================================================================
if(STOPWATCH_BUILD_EXAMPLES)
	add_executable(stopwatch_basic_usage "examples/basic_usage.cpp")
	target_link_libraries(stopwatch_basic_usage PRIVATE stopwatch)

	install(TARGETS stopwatch_basic_usage)
endif()