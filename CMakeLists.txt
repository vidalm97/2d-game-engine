cmake_minimum_required(VERSION 3.16.3)
project(2dGameEngine)

set (CMAKE_CXX_STANDARD 17)

file(GLOB SRC_FILES "src/*.cpp")
file(GLOB MODULES_SRC_FILES "src/Modules/*.cpp")

set(INC_FILES_DIR "include/")
include_directories(${INC_FILES_DIR})

add_executable(2dGameEngine ${SRC_FILES} ${MODULES_SRC_FILES})

add_subdirectory(lib/glfw)
add_subdirectory(lib/glad/src)
add_subdirectory(lib/ImGui/src)
add_subdirectory(lib/stb)

target_include_directories(2dGameEngine
	PUBLIC lib/ImGui/include
    PUBLIC lib/glfw/include
    PUBLIC lib/glad/include
    PUBLIC lib/glm
    PUBLIC lib/stb
    PUBLIC lib/json
)

target_link_directories(2dGameEngine
	PRIVATE lib/ImGui/src
    PRIVATE lib/glfw/src
    PRIVATE lib/glad/src
)
target_link_libraries(2dGameEngine glfw GLAD IMGUI stb ${SDL2_LIBRARIES})
