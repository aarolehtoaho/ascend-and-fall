#include "Utils.h"

#include <GLFW/glfw3.h>

float Utils::deltaTime = 0.0f;
float Utils::lastFrame = 0.0f;

void Utils::updateDeltaTime() {
    if (lastFrame == 0.0f) {
        lastFrame = glfwGetTime();
    }
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
};
float Utils::getDeltaTime() {
    return deltaTime;
};