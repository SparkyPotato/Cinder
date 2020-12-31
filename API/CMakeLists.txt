set(THREADS_PREFER_PTHREAD_FLAG ON)

file(GLOB_RECURSE API_SOURCE CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/Include/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)
add_library(CinderAPI SHARED ${API_SOURCE})

target_include_directories(CinderAPI PRIVATE ${PROJECT_SOURCE_DIR}/External/stb)

target_include_directories(CinderAPI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Include/Cinder)
target_include_directories(CinderAPI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/)

target_precompile_headers(CinderAPI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/PCH.h)

target_compile_features(CinderAPI PRIVATE cxx_std_17)
target_compile_definitions(CinderAPI PRIVATE BUILD_CINDER)
set_target_properties(CinderAPI PROPERTIES CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
target_link_libraries(CinderAPI PRIVATE Threads::Threads)
target_link_libraries(CinderAPI PRIVATE fmt)
target_link_libraries(CinderAPI PRIVATE yaml-cpp)
target_link_libraries(CinderAPI PRIVATE assimp)
