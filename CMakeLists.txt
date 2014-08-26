cmake_minimum_required(VERSION 2.8)

project("gyarte14-15")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-format-security -std=c++11")
set(PROJECT_NAME "gyarte")

# OpenCL
FIND_LIBRARY(OPENCL_LIBRARY OpenCL)
MARK_AS_ADVANCED(OPENCL_LIBRARY)
SET(EXTRA_LIBS ${OPENCL_LIBRARY})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# GLFW
find_package(GLFW REQUIRED)
include_directories(${GLFW_INCLUDE_DIR})

# GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIR})

# GLM
find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIR})

# local headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src" SRC_LIST)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/state" SRC_LIST)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/graphics" SRC_LIST)
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src/compute" SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS} ${OPENGL_LIBRARIES} GLEW glfw3 jsoncpp)
