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
    source/render

    libraries/glad/include
    libraries/glfw/include
    libraries/glm/include
    libraries/stb_image/include
    libraries/FastNoiseLite/include

    game
)

# Added so that glad would shut the fuck up
add_definitions(-DGLFW_INCLUDE_NONE)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

link_directories(
    libraries/glfw/lib
)

file(GLOB_RECURSE GAME_CPP_FILES game/*.cpp)
file(GLOB_RECURSE GAME_H_FILES game/*.h)

file(GLOB_RECURSE ENGINE_CPP_FILES source/*.cpp)
file(GLOB_RECURSE ENGINE_C_FILES source/*.c)


add_executable(
    ${PROJECT_NAME}

    ${GAME_CPP_FILES}
    ${GAME_H_FILES}

    ${ENGINE_CPP_FILES}
    ${ENGINE_C_FILES}

    libraries/glad/source/gl.c
    libraries/stb_image/source/stb_image.cpp
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC glfw3 
)

# Added to that OpenGL context does not start terminal
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS")