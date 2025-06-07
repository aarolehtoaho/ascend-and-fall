#ifndef WINDOW_H
#define WINDOW_H

#include "Camera.h"

class Hud;
struct GLFWwindow;
typedef unsigned int GLenum;

class Window {
private:
    Window();
    static Window* instance;
    static GLFWwindow* window;
    static Camera* camera;
    static Hud* hud;

    static unsigned int window_width;
    static unsigned int window_height;
public:
    ~Window();
    static Window* getInstance();
    static Camera* getCamera() { return camera; };
    static Hud* getHud() { return hud; };
    static float getWidth() { return static_cast<float>(window_width); };
    static float getHeight() { return static_cast<float>(window_height); };
    static float getAspectRatio();
    static void setDimensions(int width, int height);
    static bool windowShouldClose();
    static void closeWindow();
    static bool isPressed(GLenum key);
    static bool isReleased(GLenum key);
    static void update();
}; 
#endif