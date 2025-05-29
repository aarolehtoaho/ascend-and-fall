#include "Player.h"

Player::Player(glm::vec3 position, Renderer *renderer, Shader *shader, Texture *textureDiffuse, Texture *textureSpecular) \
             : Entity(position), renderer(renderer), shader(shader), textureDiffuse(textureDiffuse), textureSpecular(textureSpecular) {}
void Player::draw() {
    shader->use();
    shader->setInt("material.diffuse", textureDiffuse->getUnit());
    shader->setInt("material.specular", textureSpecular->getUnit());
    textureDiffuse->bind();
    textureSpecular->bind();

    shader->setSpotLight(glm::vec3(getPosition().x, getPosition().y, 7.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f), glm::vec2(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f))));

    renderer->drawSquare(*shader, getPosition(), glm::vec3(0.25f));
}