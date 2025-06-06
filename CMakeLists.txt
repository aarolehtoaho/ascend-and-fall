set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.10)
project(AscendAndFall)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

set(GLFW_DIR "${CMAKE_SOURCE_DIR}/include/glfw-windows-install")
set(STB_DIR "${CMAKE_SOURCE_DIR}/include/stb")
set(ASSIMP_DIR "${CMAKE_SOURCE_DIR}/include/assimp/build-windows/install")
set(ZLIB_DIR "${CMAKE_SOURCE_DIR}/include/zlib-mingw-install")

include_directories(${GLFW_DIR}/include)
include_directories(${STB_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${ASSIMP_DIR}/include)
include_directories(${ZLIB_DIR}/include)

link_directories(${GLFW_DIR}/lib)
link_directories(${ASSIMP_DIR}/lib)
link_directories(${ZLIB_DIR}/lib)

add_executable(AscendAndFall
    ${CMAKE_SOURCE_DIR}/include/glad/src/glad.c
    src/main.cpp
    src/Game.cpp
    src/Level.cpp
    src/Renderer.cpp
    src/Window.cpp
    src/Utils.cpp
    src/Input.cpp
    src/Shader.cpp
    src/Camera.cpp
    src/Texture.cpp
    src/Callbacks.cpp
    src/Entity.cpp
    src/Player.cpp
    src/Mesh.cpp
    src/Model.cpp
    src/Tile.cpp
    src/Collision.cpp
)

target_link_libraries(AscendAndFall 
    glfw3
    opengl32
    gdi32
    user32
    shell32
    assimp
    z
)
