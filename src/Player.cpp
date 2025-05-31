#include "Player.h"

Player::Player(glm::vec3 position, Model *model, Shader *shader) \
             : Entity(position, model, 0.5f, 0.125f), \
               shader(shader) {}
void Player::draw() {
    shader->use();

    float posX = getPosition().x;
    float posY = getPosition().y;
    shader->setSpotLight(glm::vec3(posX + 3, posY + 3, 2.0f), \
                         glm::vec3(-3.0f, -3.0f, -2.0f), \
                         glm::vec3(0.0f), \
                         glm::vec3(1.0f), \
                         glm::vec3(1.0f), \
                         glm::vec3(1.0f, 0.09f, 0.032f), \
                         glm::vec2(glm::cos(glm::radians(12.5f)), \
                         glm::cos(glm::radians(15.0f))));
    shader->setDirLight(glm::vec3(-0.2f, -1.0f, 0.0f), glm::vec3(0.01f), glm::vec3(0.4f), glm::vec3(0.5f));
    shader->setPointLight(0, glm::vec3(-2.0, 1.0f, 0.0f), glm::vec3(0.01f), glm::vec3(0.8f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f));
    shader->setFloat("shininess", 0.5f * 128.0f);

    glm::vec3 modelOffset = glm::vec3(0.0f, 0.05, 0.0f);
    float rotationAngle = lookingDirection == RIGHT ? 90.0f : -90.0f;

    getModel()->draw(*shader, getPosition() + modelOffset, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.3f));
}