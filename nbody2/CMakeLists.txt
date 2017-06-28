#Minimum CMake version:
cmake_minimum_required(VERSION 2.8)

project(nbody2)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/out)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_C_COMPILER clang)

find_package(OpenGL REQUIRED)

add_executable(nbody2
	main.c
	body.c
	system.c
	tipsy.c
	tree.c
	viewer/viewer.c
)



target_link_libraries(nbody2
	gsl
	${OPENGL_gl_LIBRARY}
	GLEW
	SDL2
	bsd
	m
)

add_executable(nbody2-tests
	tests/tests.c
	tests/test_vec3.c
)

target_link_libraries(nbody2-tests
	bsd
	m
)
