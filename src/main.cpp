#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Utils.h"
#include "Logger.h"
#include "Input.h"

int main() {
    Logger logger("debug.log");
    logger.logTime();

    Window* window = Window::getInstance();
    if (!window) {
        logger.log("Failed to create window instance.");
        return -1;
    }
    
    while (!Window::windowShouldClose()) {
        Utils::updateDeltaTime();

        Input::processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Window::update();
    }

    return 0;
}