cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Trusses CXX)

# Add include directories
set(INCLUDE_DIRS
	src
	src/Entities
	src/External
	src/Graphics
	src/Interface
	src/Mathematics
	src/Tools)
include_directories(${INCLUDE_DIRS})

# Find all .cpp files
foreach(DIR ${INCLUDE_DIRS})
	file(GLOB_RECURSE FILES ${DIR}/*.cpp)
	list (APPEND SRC_FILES ${FILES})
endforeach()

# Print all .cpp files
foreach(FILE ${SRC_FILES})
	MESSAGE(${FILE})
endforeach()

add_executable(Trusses ${SRC_FILES})

# Add libraries
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(Trusses ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

set(CMAKE_CXX_FLAGS "-O2")