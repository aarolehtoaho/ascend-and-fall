#ifndef WINDOW_H
#define WINDOW_H

struct GLFWwindow;
typedef unsigned int GLenum;

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
        static void closeWindow();
        static bool isPressed(GLenum key);
        static void update();
};
#endif