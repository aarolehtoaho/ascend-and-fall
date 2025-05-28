#include "Callbacks.h"
#include "Window.h"

Camera* Callbacks::camera = nullptr;

void Callbacks::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    Window::getInstance()->setDimensions(width, height);
}
void Callbacks::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    Window::setMousePositions(xpos, ypos);
}
void Callbacks::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera->processMouseScroll(yoffset);
}
void Callbacks::setCallbacks(GLFWwindow* window, Camera* camera) {
    Callbacks::camera = camera;
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
}