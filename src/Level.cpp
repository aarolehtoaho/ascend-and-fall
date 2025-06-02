#include "Level.h"
#include "Tile.h"
#include "Entity.h"
#include "Logger.h"
#include "Renderer.h"
#include "Model.h"

#include <glm/glm.hpp>

Logger Level::logger("debug.log");

const int CHUNK_SIZE = 16;

Level::Level(levelName name)
            : name(name),
              shapeShader("assets/shaders/shape_phong.vs", "assets/shaders/shape_phong.fs"),
              modelShader("assets/shaders/modelshader.vs", "assets/shaders/modelshader.fs"),
              backgroundTexture("assets/textures/background_forest.jpg"),
              backgroundSpecularTexture("assets/textures/no_specular.png") {
    switch (name) {
        case FOREST:
            createForest();
            break;
        case POISONOUS_HOLLOW:
        case SCORCHING_DUNES:
        case NIHMIRS_TOMB:
        case MISTY_CAVES:
        case FROST_BREACH:
        case MOLTEN_DEPTHS:
        case CRYPT_OF_AETHEROS:
        case AETHEROS:
        default:
            logger.log("Level not implemented yet.");
            throw std::runtime_error("Level not implemented yet.");
    }
}

void Level::addTile(Tile tile) {
    std::pair<int, int> chunkCoords = getChunkCoordinates(tile.getPositionX(), tile.getPositionY());
    chunkTiles[chunkCoords].push_back(tile);
}
void Level::addEntity(Entity entity) {
    std::pair<int, int> chunkCoords = getChunkCoordinates(entity.getPosition().x, entity.getPosition().y);
    chunkEntities[chunkCoords].push_back(entity);
}

std::pair<int, int> Level::getChunkCoordinates(float posX, float posY) const {
    std::pair<int, int> chunkCoordinates;
    chunkCoordinates.first = floor(posX / CHUNK_SIZE);
    chunkCoordinates.second = floor(posY / CHUNK_SIZE);
    return chunkCoordinates;
}

void Level::createForest() {
    leftBound = -123.0f;
    rightBound = 123.0f;
    bottomBound = 0.0f;
    topBound = 512.0f;

    tileModels.emplace_back("assets/models/testmodel/testCube/testCube.obj");
    Model *testModel = &tileModels.back();

    Tile tile1(0, 0, glm::vec2(1.0f, 1.0f), TILE_SOLID, testModel);
    Tile tile2(1, 1, glm::vec2(1.0f, 1.0f), TILE_SOLID, testModel);
    Tile tile3(-1, 0, glm::vec2(1.0f, 1.0f), TILE_SOLID, testModel);
    Tile tile4(-123, 0, glm::vec2(1.0f, 1.0f), TILE_SOLID, testModel);
    Tile tile5(123, 0, glm::vec2(1.0f, 1.0f), TILE_SOLID, testModel);

    addTile(tile1);
    addTile(tile2);
    addTile(tile3);
    addTile(tile4);
    addTile(tile5); 
}
void Level::render(Renderer *renderer, Shader *shapeShader, Shader *modelShader) {
    renderBackground(renderer, shapeShader);

    for (auto& it: chunkTiles) {
        for (Tile& tile: it.second) {
            tile.render(modelShader);
        }
    }
}

void Level::renderBackground(Renderer* renderer, Shader *shader) {
    shader->use();

    shader->setInt("material.diffuse", backgroundTexture.getUnit());
    shader->setInt("material.specular", backgroundSpecularTexture.getUnit());
    backgroundTexture.bind();
    backgroundSpecularTexture.bind();

    shader->setDirLight(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.05f), glm::vec3(1.0f), glm::vec3(0.0f));
    shader->setPointLight(0, glm::vec3(1.0f), glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f));
    shader->setSpotLight(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f), glm::vec2(glm::cos(glm::radians(0.0f)), glm::cos(glm::radians(0.0f))));

    renderer->drawSquare(*shader, glm::vec3(0.0f, 255.0f, -100.0f), glm::vec3(630.0f, 680.0f, 1.0f));    
}