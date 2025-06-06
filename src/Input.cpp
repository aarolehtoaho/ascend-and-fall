#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.h"
#include "Utils.h"
#include "Window.h"

bool Input::hasReleasedW = true;

void Input::processInput(Player *player) {
    float deltaTime = Utils::getDeltaTime();

    if (Window::isPressed(GLFW_KEY_ESCAPE)) {
        Window::closeWindow();
    }

    if (Window::isPressed(GLFW_KEY_W) && hasReleasedW && player->isOnGround()) {
        hasReleasedW = false;
        player->applyImpulse(glm::vec3(0.0f, 1.2f, 0.0f));
        player->setOnGround(false);
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
    if (Window::isPressed(GLFW_KEY_UP)) {
        player->applyForce(glm::vec3(0.0f, 4.0f, 0.0f));
    }
    if (Window::isReleased(GLFW_KEY_W)) {
        hasReleasedW = true;
    }
}