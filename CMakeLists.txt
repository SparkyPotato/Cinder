cmake_minimum_required(VERSION 3.16)

project(Cinder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Cinder/Dependencies/fmt)
add_subdirectory(Cinder/Dependencies/yaml-cpp)

include_directories(${PROJECT_SOURCE_DIR}/Cinder/Source/)
include_directories(${PROJECT_SOURCE_DIR}/Cinder/Dependencies/fmt/include)
include_directories(${PROJECT_SOURCE_DIR}/Cinder/Dependencies/yaml-cpp/include)
include_directories(${PROJECT_SOURCE_DIR}/Cinder/Dependencies/stb)

file(GLOB_RECURSE CINDER_SOURCE CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/Cinder/Source/*.cpp)
add_executable(Cinder ${CINDER_SOURCE})

target_precompile_headers(Cinder PRIVATE ${PROJECT_SOURCE_DIR}/Cinder/Source/PCH.h)

target_link_libraries(Cinder PRIVATE fmt)
target_link_libraries(Cinder PRIVATE yaml-cpp)
