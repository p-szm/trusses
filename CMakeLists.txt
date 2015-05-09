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

set(SRC_FILES "")
set(H_FILES "")
foreach(DIR ${INCLUDE_DIRS})
	# Find all .h and .cpp files
	file(GLOB_RECURSE DIR_CPP_FILES ${DIR}/*.cpp)
	file(GLOB_RECURSE DIR_H_FILES ${DIR}/*.h)

	list (APPEND SRC_FILES ${DIR_CPP_FILES})
	list (APPEND H_FILES ${DIR_H_FILES})

	# Group the files
	string(FIND ${DIR} "/" POS)
	if (NOT POS EQUAL -1)
		MATH(EXPR POS "${POS}+1")
		string(SUBSTRING ${DIR} ${POS} -1 DIR_NAME)
	else()
		set(DIR_NAME Source)
	endif()
	
	source_group(${DIR_NAME} FILES ${DIR_CPP_FILES} ${DIR_H_FILES})
endforeach()

# Print all .cpp files
foreach(FILE ${SRC_FILES})
	MESSAGE(${FILE})
endforeach()

add_executable(Trusses ${SRC_FILES} ${H_FILES})

# Add libraries
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(Trusses ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

#set(CMAKE_CXX_FLAGS "-O2")