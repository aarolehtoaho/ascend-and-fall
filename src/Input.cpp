#include <GLFW/glfw3.h>

#include "Input.h"
#include "Utils.h"
#include "Window.h"

void Input::processInput(Player *player) {
    float deltaTime = Utils::getDeltaTime();

    if (Window::isPressed(GLFW_KEY_ESCAPE)) {
        Window::closeWindow();
    }

    if (Window::isPressed(GLFW_KEY_W) && !player->isJumping()) {
        player->applyImpulse(glm::vec3(0.0f, 1.0f, 0.0f));
        player->setJumping(true);
    }
    if (Window::isPressed(GLFW_KEY_A)) {
        player->applyForce(glm::vec3(-1.0f, 0.0f, 0.0f));
    }
    if (Window::isPressed(GLFW_KEY_S)) {
        // crouch
    }
    if (Window::isPressed(GLFW_KEY_D)) {
        player->applyForce(glm::vec3(1.0f, 0.0f, 0.0f));
    }
}