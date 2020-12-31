cmake_minimum_required(VERSION 3.16)

cmake_policy(SET CMP0077 NEW)

project (Cinder LANGUAGES CXX)

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
	option(SUBMODULE "Check submodules during build" ON)
	if(SUBMODULE)
		execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
			RESULT_VARIABLE GIT_SUBMOD_RESULT)
		if(NOT GIT_SUBMOD_RESULT EQUAL "0")
			message(FATAL_ERROR "Updating submodules failed with code ${GIT_SUBMOD_RESULT}.")
		endif()
	endif()
endif()

if (NOT WIN32)
	add_compile_options("-fPIC")
endif ()

option(BUILD_DOCS "Build the documentation" ON)
option(BUILD_CLI "Build the CLI tool" ON)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libraries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Libraries)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)

option(ASSIMP_BUILD_TESTS OFF "")
option(BUILD_SHARED_LIBS OFF "")
option(YAML_BUILD_SHARED_LIBS OFF "")

add_subdirectory(${PROJECT_SOURCE_DIR}/External/fmt)
add_subdirectory(${PROJECT_SOURCE_DIR}/External/yaml-cpp)
add_subdirectory(${PROJECT_SOURCE_DIR}/External/Assimp)

add_subdirectory(API)

if (BUILD_CLI)
	add_subdirectory(CLI)
endif ()

if (BUILD_DOCS)
	add_subdirectory(Docs)
endif ()
