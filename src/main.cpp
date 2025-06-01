#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Window.h"
#include "Utils.h"
#include "Logger.h"
#include "Input.h"
#include "Camera.h"
#include "Renderer.h"
#include "Texture.h"
#include "Player.h"
#include "Model.h"
#include "Tile.h"
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

    Shader defaultShader("assets/shaders/shape_phong.vs", "assets/shaders/shape_phong.fs");
    Shader modelShader("assets/shaders/modelshader.vs", "assets/shaders/modelshader.fs");

    Texture container("assets/textures/container.png");
    Texture containerSpecular("assets/textures/container_specular.png");
    Texture background_forest("assets/textures/background_forest.png");
    Texture no_specular("assets/textures/no_specular.png");

    Model playerModel("assets/models/player/player_texture.obj");
    Player player(glm::vec3(0.0f, 5.0f, 0.0f), &playerModel, &modelShader);

    Model testModel("assets/models/testmodel/testCube/testCube.obj");

    Tile tile1(0, 0, glm::vec2(1.0f, 1.0f), TILE_SOLID, &testModel);
    Tile tile2(1, 1, glm::vec2(1.0f, 1.0f), TILE_SOLID, &testModel);
    Tile tile3(-1, 0, glm::vec2(1.0f, 1.0f), TILE_SOLID, &testModel);
    Tile tile4(-123, 0, glm::vec2(1.0f, 1.0f), TILE_SOLID, &testModel);
    Tile tile5(123, 0, glm::vec2(1.0f, 1.0f), TILE_SOLID, &testModel);

    while (!Window::windowShouldClose()) {
        Utils::updateDeltaTime();
        float deltaTime = Utils::getDeltaTime();

        Input::processInput(&player);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        defaultShader.use();

        defaultShader.setInt("material.diffuse", background_forest.getUnit());
        defaultShader.setInt("material.specular", no_specular.getUnit());
        background_forest.bind();
        no_specular.bind();

        defaultShader.setDirLight(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(0.0f));
        defaultShader.setSpotLight(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f), glm::vec2(glm::cos(glm::radians(0.0f)), glm::cos(glm::radians(0.0f))));

        renderer.drawSquare(defaultShader, glm::vec3(0.0f, 200.0f, -100.0f), glm::vec3(600.0f, 600.0f, 1.0f));

        defaultShader.setInt("material.diffuse", container.getUnit());
        defaultShader.setInt("material.specular", containerSpecular.getUnit());        
        container.bind();
        containerSpecular.bind();

        defaultShader.setDirLight(glm::vec3(-0.2f, -1.0f, 0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
        defaultShader.setPointLight(0, glm::vec3(1.0f), glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f));
        defaultShader.setSpotLight(glm::vec3(-3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f), glm::vec2(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f))));
        
        renderer.drawCube(defaultShader, glm::vec3(-4.0f, 0.0f, 0.0f), glm::vec3(0.5f));
        renderer.drawCube(defaultShader, glm::vec3(-3.5f, 0.0f, 0.0f), glm::vec3(0.5f));

        player.draw();
        player.update(deltaTime);
        camera->moveCamera(player.getPosition(), player.getMovementSpeed() + 1.0f, deltaTime);

        tile1.render(&modelShader);
        tile2.render(&modelShader);
        tile3.render(&modelShader);
        tile4.render(&modelShader);
        tile5.render(&modelShader);

        Window::update();
    }

    return 0;
}