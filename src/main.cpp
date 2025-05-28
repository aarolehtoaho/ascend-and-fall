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

    Texture container("assets/textures/container.png");
    Texture containerSpecular("assets/textures/container_specular.png");

    defaultShader.use();
    defaultShader.setInt("material.diffuse", container.getUnit());
    defaultShader.setInt("material.specular", containerSpecular.getUnit());
    
    while (!Window::windowShouldClose()) {
        Utils::updateDeltaTime();

        Input::processInput(camera);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        container.bind();
        containerSpecular.bind();

        defaultShader.use();

        defaultShader.setDirLight(glm::vec3(-0.2f, -1.0f, 0.3f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
        defaultShader.setPointLight(0, glm::vec3(1.0f), glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f));
        defaultShader.setSpotLight(glm::vec3(-3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f), glm::vec2(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f))));
        
        renderer.drawCube(defaultShader, glm::vec3(0.0f), glm::vec3(1.0f));

        Window::update();
    }

    return 0;
}