#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Window.h"
#include "Utils.h"
#include "Logger.h"
#include "Input.h"
#include "Camera.h"
#include "Renderer.h"
#include "Player.h"
#include "Model.h"
#include "Level.h"
//#include "Animation.h"
//#include "Animator.h"

int main() {
    Logger logger("debug.log");
    logger.logTime();

    Window* window = Window::getInstance();
    if (!window) {
        logger.log("Failed to create window instance.");
        return -1;
    }
    Camera* camera = window->getCamera();
    Renderer renderer(camera);

    Model playerModel("assets/models/player/player_texture.obj");
    Player player(glm::vec3(0.0f, 5.0f, 0.0f), &playerModel);

    Level level(FOREST);

    while (!Window::windowShouldClose()) {
        Utils::updateDeltaTime();

        Input::processInput(&player);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        level.render(&renderer, camera, player.getPosition());

        player.draw();
        player.update();

        camera->moveCamera(player.getPosition(), player.getMovementSpeed() + 1.0f);

        Window::update();
    }

    return 0;
}