#include "Player.h"

Player::Player(glm::vec3 position, Renderer *renderer, Shader *shader, Texture *textureDiffuse, Texture *textureSpecular) \
             : Entity(position), \
             renderer(renderer), \
             shader(shader), \
             textureDiffuse(textureDiffuse), \
             textureSpecular(textureSpecular) {}
void Player::draw() {
    shader->use();
    shader->setInt("material.diffuse", textureDiffuse->getUnit());
    shader->setInt("material.specular", textureSpecular->getUnit());
    textureDiffuse->bind();
    textureSpecular->bind();

    float posX = getPosition().x;
    float posY = getPosition().y;
    float offsetY = height / 4.0f;
    shader->setSpotLight(glm::vec3(posX, posY, 7.0f), \
                         glm::vec3(0.0f, 0.0f, -1.0f), \
                         glm::vec3(0.0f), \
                         glm::vec3(1.0f), \
                         glm::vec3(1.0f), \
                         glm::vec3(1.0f, 0.09f, 0.032f), \
                         glm::vec2(glm::cos(glm::radians(12.5f)), \
                         glm::cos(glm::radians(15.0f))));

    renderer->drawSquare(*shader, glm::vec3(posX, posY - offsetY, 0.0f), glm::vec3(width));
    renderer->drawSquare(*shader, glm::vec3(posX, posY + offsetY, 0.0f), glm::vec3(width));
}