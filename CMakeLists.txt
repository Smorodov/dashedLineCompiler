cmake_minimum_required(VERSION 3.8)
cmake_policy(SET CMP0063 NEW) # Honor visibility properties.

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

project(dashedLineCompiler)

# Some basics, set accordingly to your needs.
set(CMAKE_CXX_STANDARD 17)
set(BLEND2D_DIR "${CMAKE_SOURCE_DIR}/blend2d" CACHE PATH "Location of 'blend2d'")

# Blend2D
# -------

set(BLEND2D_STATIC TRUE)
include("${BLEND2D_DIR}/CMakeLists.txt")

include_directories(${CMAKE_SOURCE_DIR})
set(SOURCES 
main.cpp
)
  add_executable(${PROJECT_NAME} ${SOURCES})
  set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_VISIBILITY_PRESET hidden)
  target_link_libraries(${PROJECT_NAME} Blend2D::Blend2D )

set_target_properties( ${PROJECT_NAME} PROPERTIES  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
