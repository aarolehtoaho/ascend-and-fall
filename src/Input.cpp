#include <GLFW/glfw3.h>

#include "Input.h"
#include "Utils.h"
#include "Window.h"

void Input::processInput() {
    float deltaTime = Utils::getDeltaTime();

    if (Window::isPressed(GLFW_KEY_ESCAPE)) {
        Window::closeWindow();
    }
}