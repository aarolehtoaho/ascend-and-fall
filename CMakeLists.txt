cmake_minimum_required(VERSION 3.10)
project(AscendAndFall)

set(CMAKE_CXX_STANDARD 17)

set(GLFW_DIR "./glfw-windows-install")

set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(${GLFW_DIR}/include)
include_directories(include)
link_directories(${GLFW_DIR}/lib)

add_executable(ascendra 
    src/main.cpp
    src/glad.c
    src/Game.cpp
    src/Renderer.cpp
    src/Window.cpp
)

target_link_libraries(AscendAndFall 
    glfw3
    opengl32
    gdi32
    user32
    shell32
)
