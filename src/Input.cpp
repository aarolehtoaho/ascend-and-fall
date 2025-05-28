#include <GLFW/glfw3.h>

#include "Input.h"
#include "Utils.h"
#include "Window.h"

void Input::processInput(Camera *camera) {
    float deltaTime = Utils::getDeltaTime();

    if (Window::isPressed(GLFW_KEY_ESCAPE)) {
        Window::closeWindow();
    }

    if (Window::isPressed(GLFW_KEY_W)) {
        camera->processKeyboard(UP, deltaTime);
    }
    if (Window::isPressed(GLFW_KEY_A)) {
        camera->processKeyboard(LEFT, deltaTime);
    }
    if (Window::isPressed(GLFW_KEY_S)) {
        camera->processKeyboard(DOWN, deltaTime);
    }
    if (Window::isPressed(GLFW_KEY_D)) {
        camera->processKeyboard(RIGHT, deltaTime);
    }
}