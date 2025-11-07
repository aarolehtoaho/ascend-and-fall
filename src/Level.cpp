#include "Level.h"
#include "Entity.h"
#include "Logger.h"
#include "Renderer.h"
#include "Model.h"
#include "Collision.h"
#include "Player.h"

#include <glm/glm.hpp>

Logger Level::logger("debug.log");

const int CHUNK_SIZE = 16;

Level::Level(levelName name, Player *player)
            : name(name),
              shapeShader("assets/shaders/shape_phong.vs", "assets/shaders/shape_phong.fs"),
              modelShader("assets/shaders/modelshader.vs", "assets/shaders/modelshader.fs"),
              backgroundTexture("assets/textures/background_forest.jpg"),
              backgroundSpecularTexture("assets/textures/no_specular.png"),
              player(player) {
    Tile::setPlayer(player);

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

    tileModels.emplace_back("assets/models/groundTile/groundTile.obj");
    Model *testModel = &tileModels.back();

    Tile tileAtLeftBorder(-123, 0, glm::vec2(1.0f, 1.0f), TILE_SOLID, testModel);
    Tile tileAtRightBorder(123, 0, glm::vec2(1.0f, 1.0f), TILE_SOLID, testModel);
    addTile(tileAtLeftBorder);
    addTile(tileAtRightBorder);

    // Generate random sized blocks to random locations
    unsigned int seed = 1337;
    srand(seed);
    for (int block = 0; block < 200; block++) {
        int x = rand() % (int)(rightBound + 1);
        x = rand() % 2 == 0 ? x : -x;
        int y = rand() % (int)(topBound + 1);
        int blockWidth = rand() % 15;
        int blockHeight = rand() % 8;
        for (int xOffset = 0; xOffset < blockWidth && x + xOffset <= rightBound; xOffset++) {
            for (int yOffset = 0; yOffset < blockHeight && y + yOffset <= topBound; yOffset++) {
                Tile generatedTile(x + xOffset, y + yOffset, glm::vec2(1.0f, 1.0f), TILE_SOLID, testModel);
                addTile(generatedTile);
            }
        }
    }
}
void Level::render(Renderer *renderer, Camera *camera, glm::vec3 playerPosition) {
    renderBackground(renderer, &shapeShader);

    std::set<std::pair<int, int>> renderedChunks = chunksToRender(camera);

    for (auto& chunk: renderedChunks) {
        for (Tile& tile: chunkTiles[chunk]) {
            tile.render(&modelShader);
        }
    }
}

void Level::update() {
    player->setOnGround(false);
    checkCollisions(player);
    player->checkMapBounds();
}

void Level::checkCollisions(Entity *entity) {
    AABB entityAABB = entity->getAABB();
    
    for (int xOffset = -1; xOffset <= 1; xOffset++) {
        for (int yOffset = -1; yOffset <= 1; yOffset++) {
            for (Tile& tile: chunkTiles[getChunkCoordinates(entity->getPosition().x + xOffset, entity->getPosition().y + yOffset)]) {
                if (tile.getAABB().intersects(entityAABB)) {
                    entity->handleCollision(&tile);
                }
            }
        }
    }
}

bool Level::chunkInsideBounds(int chunkX, int chunkY) {
    return (chunkX >= floor(leftBound / CHUNK_SIZE) && chunkX <= floor(rightBound / CHUNK_SIZE) && chunkY >= floor(bottomBound / CHUNK_SIZE) && chunkY <= floor(topBound / CHUNK_SIZE));
}

std::set<std::pair<int, int>> Level::chunksToRender(Camera *camera) {
    std::set<std::pair<int, int>> result;

    std::pair<int, int> cameraChunk = getChunkCoordinates(camera->getPosition().x, camera->getPosition().y);
    int rangeX = ceil(camera->distanceToSideEdge() / CHUNK_SIZE);
    int rangeY = ceil(camera->distanceToBottomEdge() / CHUNK_SIZE);
    for (int xOffset = -rangeX; xOffset <= rangeX; xOffset++) {
        for (int yOffset = -rangeY; yOffset <= rangeY; yOffset++) {
            int chunkX = cameraChunk.first + xOffset;
            int chunkY = cameraChunk.second + yOffset;
            if (chunkInsideBounds(chunkX, chunkY)) {
                std::pair<int, int> chunk;
                chunk.first = chunkX;
                chunk.second = chunkY;
                result.insert(chunk);
            }
        }
    }

    return result;
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