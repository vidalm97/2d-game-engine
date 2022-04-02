cmake_minimum_required(VERSION 3.16.3)
project(2dGameEngine)

file(GLOB SRC_FILES "src/*.cpp")

set(INC_FILES_DIR "include/")

include_directories(${INC_FILES_DIR})

add_executable(2dGameEngine ${SRC_FILES})
