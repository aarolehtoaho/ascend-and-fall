#include "Player.h"

Player::Player(glm::vec3 position, Model *model) \
             : Entity(position, 2.0f, 1.1f, model), \
               shader("assets/shaders/modelshader.vs", "assets/shaders/modelshader.fs") {}

void Player::draw() {
    setLights();

    getModel()->draw(shader, getPosition(), getRotationAngle(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Player::setLights() {
    shader.use();

    shader.setSpotLight(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f), glm::vec2(glm::cos(0.0f), glm::cos(0.0f)));
    shader.setDirLight(glm::vec3(0.3f, -1.0f, 0.2f), glm::vec3(0.01f), glm::vec3(0.4f), glm::vec3(0.5f));
    shader.setPointLight(0, getPosition() + glm::vec3(-0.3f, 1.0f, 1.0f), glm::vec3(0.01f), glm::vec3(0.8f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f));
    shader.setFloat("shininess", 0.5f * 128.0f);
}