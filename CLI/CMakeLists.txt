file(GLOB_RECURSE CLI_SOURCE CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)
add_executable(cinder ${CLI_SOURCE})

target_include_directories(cinder PRIVATE ${PROJECT_SOURCE_DIR}/API/Include)
target_include_directories(cinder PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Source/)

target_compile_features(cinder PUBLIC cxx_std_17)
set_target_properties(cinder PROPERTIES CXX_EXTENSIONS OFF)
target_link_libraries(cinder PRIVATE CinderAPI)

target_include_directories(cinder PRIVATE ${PROJECT_SOURCE_DIR}/External/fmt/include)
target_link_libraries(cinder PUBLIC fmt)
