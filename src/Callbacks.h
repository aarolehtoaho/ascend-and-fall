#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera;
class Hud;

class Callbacks {
private:
    static Camera* camera;
    static Hud* hud;
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
public:
    static void setCallbacks(GLFWwindow* window, Camera* camera, Hud* hud);
};
#endif