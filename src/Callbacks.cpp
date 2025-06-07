#include "Callbacks.h"
#include "Camera.h"
#include "Window.h"
#include "Hud.h"

Camera* Callbacks::camera = nullptr;
Hud* Callbacks::hud = nullptr;

void Callbacks::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Window::getInstance()->setDimensions(width, height);
}
void Callbacks::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    hud->setCrosshairPosition(xpos, ypos);
}
void Callbacks::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera->processMouseScroll(yoffset);
}
void Callbacks::setCallbacks(GLFWwindow* window, Camera* camera, Hud* hud) {
    Callbacks::camera = camera;
    Callbacks::hud = hud;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}