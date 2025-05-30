#ifndef WINDOW_H
#define WINDOW_H

#include "Camera.h"

struct GLFWwindow;
typedef unsigned int GLenum;

class Window {
    private:
        Window();
        static Window* instance;
        static GLFWwindow* window;
        static Camera* camera;
        static unsigned int window_width;
        static unsigned int window_height;
        static double mouse_x, mouse_y;
    public:
        ~Window();
        static Window* getInstance();
        static Camera* getCamera() { return camera; };
        static float getAspectRatio();
        static void setDimensions(int width, int height);
        static void setMousePositions(double xpos, double ypos);
        static bool windowShouldClose();
        static void closeWindow();
        static bool isPressed(GLenum key);
        static bool isReleased(GLenum key);
        static void update();
}; 
#endif