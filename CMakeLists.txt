cmake_minimum_required(VERSION 3.16)

project(Cinder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Cinder/Dependencies/fmt)
add_subdirectory(Cinder/Dependencies/yaml-cpp)
add_subdirectory(Cinder/Dependencies/Assimp)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/Executable)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(${PROJECT_SOURCE_DIR}/Cinder/Source/)
include_directories(${PROJECT_SOURCE_DIR}/Cinder/Dependencies/fmt/include)
include_directories(${PROJECT_SOURCE_DIR}/Cinder/Dependencies/yaml-cpp/include)
include_directories(${PROJECT_SOURCE_DIR}/Cinder/Dependencies/stb)
include_directories(${PROJECT_SOURCE_DIR}/Cinder/Dependencies/Assimp/include)

file(GLOB_RECURSE CINDER_SOURCE CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/Cinder/Source/*.cpp)
add_executable(Cinder ${CINDER_SOURCE})

target_precompile_headers(Cinder PRIVATE ${PROJECT_SOURCE_DIR}/Cinder/Source/PCH.h)

find_package(Threads REQUIRED)
target_link_libraries(Cinder PRIVATE Threads::Threads)
target_link_libraries(Cinder PRIVATE fmt)
target_link_libraries(Cinder PRIVATE yaml-cpp)
