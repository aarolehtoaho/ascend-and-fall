#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
    private:
        Window();
        static Window* instance;
        static GLFWwindow* window;
        static unsigned int window_width;
        static unsigned int window_height;
    public:
        ~Window();
        static Window* getInstance();
        static float getAspectRatio();
        static void setDimensions(int width, int height);
        static bool windowShouldClose();
        static bool isPressed(GLenum key);
};
#endif