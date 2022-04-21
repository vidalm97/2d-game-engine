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

add_subdirectory(lib/glfw)
add_subdirectory(lib/glad/src)

target_include_directories(2dGameEngine
    PUBLIC lib/glfw/include
    PUBLIC lib/glad/include
)

target_link_directories(2dGameEngine
    PRIVATE lib/glfw/src
    PRIVATE lib/glad/src
)
target_link_libraries(2dGameEngine glfw GLAD ${SDL2_LIBRARIES})
