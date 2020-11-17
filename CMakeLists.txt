#Cmake verson
cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++11)

#Project name
project(BMP)

#Add search paths for multiple specified header files, separated by Spaces, to the project.
include_directories(include)

#Stores all source files in the specified directory in a single variable.
aux_source_directory(src SRC_LIST)

#The final elf file to be generated is called XXX, and the source file is XXX.
add_executable(main ${SRC_LIST})

#EXECUTABLE_OUTPUT_PATH: the location of the target binary executable.
#PROJECT_SOURCE_DIR: the root directory of the project.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})