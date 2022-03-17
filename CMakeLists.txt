cmake_minimum_required(VERSION 3.16)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${PROJECT_NAME})

if(${CMAKE_CL_64})
  set(CMAKE_64_BIT_POSTFIX -x64)
endif(${CMAKE_CL_64})
set(CMAKE_DEBUG_POSTFIX -debug)

include_directories(
    source
    source/comet

    libraries/glad/include
    libraries/glfw/include
    libraries/glm/include
    libraries/imgui/include
    libraries/stb_image/include

    game
)

# Added so that glad would shut the fuck up
add_definitions(-DGLFW_INCLUDE_NONE)

link_directories(
    libraries/glfw/lib
)

file(GLOB_RECURSE CPP_FILES source/*.cpp)
file(GLOB_RECURSE C_FILES source/*.c)
file(GLOB_RECURSE HPP_FILES source/*.hpp)
file(GLOB_RECURSE H_FILES source/*.h)
file(GLOB_RECURSE IMGUI_CPP_FILES libraries/imgui/source/*.cpp)

add_executable(
    ${PROJECT_NAME}
    ${CPP_FILES}
    ${C_FILES}
    ${HPP_FILES}
    ${H_FILES}
    libraries/glad/source/gl.c
    ${IMGUI_CPP_FILES}
    libraries/stb_image/source/stb_image.cpp

    game/Main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}

    debug glfw3.lib
    optimized glfw3.lib
)

# Added to that OpenGL context does not start terminal
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")