cmake_minimum_required(VERSION 3.16.3)
project(2dGameEngine)

file(GLOB SRC_FILES "src/*.cpp")

set(INC_FILES_DIR "include/")
set(INC_IMGUI_FILES_DIR "lib/ImGui/include/")
include_directories(${INC_FILES_DIR})
include_directories(${INC_IMGUI_FILES_DIR})

find_package(SDL2 REQUIRED)
include_directories(2dGameEngine ${SDL2_INCLUDE_DIRS})

add_executable(2dGameEngine ${SRC_FILES})
target_link_libraries(2dGameEngine ${SDL2_LIBRARIES})
