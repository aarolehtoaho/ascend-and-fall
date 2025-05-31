#include "Tile.h"

#include "Model.h"
#include "Shader.h"

int Tile::tileCount = 0;

Tile::Tile(int posX, int posY, glm::vec2 size, TileType type, Model* model)
    : positionX(posX), positionY(posY), size(size), type(type), model(model) {
    tileID = tileCount++;
}

void Tile::render(Shader *shader) {
    shader->use();

    setLights(shader);
    
    model->draw(*shader, glm::vec3(positionX, positionY, 0.0f), 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Tile::setLights(Shader *shader) {
    shader->setDirLight(glm::vec3(0.3f, -1.0f, 0.6f), glm::vec3(0.05f), glm::vec3(0.4f), glm::vec3(0.5f));
    shader->setPointLight(0, glm::vec3(1.0f), glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f));
    shader->setSpotLight(glm::vec3(-3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f), glm::vec2(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f))));
    shader->setFloat("shininess", 0.5f * 128.0f);
}