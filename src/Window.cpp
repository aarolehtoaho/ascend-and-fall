#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Logger.h"
#include "Callbacks.h"

#include <stdexcept>

Window* Window::instance = nullptr;
GLFWwindow* Window::window = nullptr;
Camera* Window::camera = nullptr;
unsigned int Window::window_width = 1280;
unsigned int Window::window_height = 720;
double Window::mouse_x = 0;
double Window::mouse_y = 0;

Window::Window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(window_width, window_height, "Ascend And Fall", NULL, NULL);

    if (window == NULL)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    camera = new Camera(glm::vec3(0.0f, 0.0f, 15.0f));

    Callbacks::setCallbacks(window, camera);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Change to hidden after implementing custom cursor
};
Window* Window::getInstance() {
    if (instance == nullptr) {
        try {
            instance = new Window();
        }
        catch (const std::runtime_error& e) {
            Logger logger("debug.log");
            logger.log(e.what());
            return nullptr;
        }
    }
    return instance;
};
float Window::getAspectRatio() {
    return (float)window_width / (float)window_height;
};
void Window::setDimensions(int width, int height) {
    glViewport(0, 0, width, height);
    window_width = width;
    window_height = height;
};
void Window::setMousePositions(double xpos, double ypos) {
    mouse_x = xpos;
    mouse_y = ypos;
}

bool Window::windowShouldClose() {
    return glfwWindowShouldClose(window);
};
void Window::closeWindow() {
    glfwSetWindowShouldClose(window, true);
}

bool Window::isPressed(GLenum key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
};
bool Window::isReleased(GLenum key) {
    return glfwGetKey(window, key) == GLFW_RELEASE;
};

void Window::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
};