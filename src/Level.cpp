#include "Level.h"
#include "Entity.h"
#include "Logger.h"
#include "Renderer.h"
#include "Model.h"
#include "Collision.h"
#include "Player.h"
#include "Perlin2D.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Logger Level::logger("debug.log");

const int CHUNK_SIZE = 8;
const bool FRONT_LAYER = true;
const bool BACK_LAYER = true;

Level::Level(levelName name, Player *player)
            : name(name),
              shapeShader("assets/shaders/shape_phong.vs", "assets/shaders/shape_phong.fs"),
              groundShader("assets/shaders/instancedshader.vs", "assets/shaders/shape_phong.fs"),
              modelShader("assets/shaders/modelshader.vs", "assets/shaders/modelshader.fs"),
              backgroundTexture("assets/textures/background_forest.jpg"),
              backgroundSpecularTexture("assets/textures/no_specular.png"),
              player(player),
              groundTexture("assets/textures/ground.jpg"),
              groundSpecularTexture("assets/textures/no_specular.png") {
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

bool Level::addTile(Tile tile, bool layer = FRONT_LAYER) {
    std::pair<int, int> chunkCoords = getChunkCoordinates(tile.getPositionX(), tile.getPositionY());
    std::pair<int, int> tileCoords = {tile.getPositionX(), tile.getPositionY()};
    bool tileAdded = (FRONT_LAYER ? frontLayerChunks[chunkCoords] : backLayerChunks[chunkCoords])
                        .emplace(tileCoords, std::move(tile)).second;
    return tileAdded;
}

Tile* Level::getTile(int x, int y, bool layer = FRONT_LAYER) {
    std::pair<int, int> chunkCoords = getChunkCoordinates(x, y);
    std::pair<int, int> tileCoords = {x, y};

    auto chunkIt = FRONT_LAYER ? frontLayerChunks.find(chunkCoords) : backLayerChunks.find(chunkCoords);
    auto end = FRONT_LAYER ? frontLayerChunks.end() : backLayerChunks.end();
    if (chunkIt == end) {
        return nullptr;
    }

    auto tileMap = chunkIt->second;
    auto result = tileMap.find(tileCoords);
    if (result == tileMap.end()) {
        return nullptr;
    }
    return &result->second;
}

void Level::addEntity(Entity entity) {
    std::pair<int, int> chunkCoords = getChunkCoordinates(entity.getPosition().x, entity.getPosition().y);
    entityChunks[chunkCoords].push_back(entity);
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
    unsigned int levelWidth = rightBound - leftBound;
    unsigned int levelHeight = topBound - bottomBound;

    tileModels.reserve(2);
    //tileModels.emplace_back("assets/models/groundTile/groundTile.obj");
    //Model *groundTileModel = &tileModels.back();
    //Tile randomTile(0.0f, 2.0f, glm::vec2(1.0f, 1.0f), TILE_SOLID, groundTileModel);
    //addTile(randomTile);

    unsigned int seed = 1337;
    unsigned int tile_size = 8;
    unsigned int perlinWidth = levelWidth / tile_size;
    unsigned int perlinHeight = levelHeight / tile_size;
    Perlin2D noiseMap(perlinWidth, perlinHeight, seed);

    float pivotValueForTile = 0.1f;
    for (int level_x = leftBound; level_x <= rightBound; level_x++) {
        for (int level_y = bottomBound; level_y <= topBound; level_y++) {
            float noiseValue = noiseMap.getNoise((level_x - leftBound) / tile_size, (level_y - bottomBound) / tile_size);
            bool isBorderTile = level_x == leftBound || level_x == rightBound || level_y == bottomBound || level_y == topBound;
            if (noiseValue > pivotValueForTile || isBorderTile) {
                Tile generatedTile(level_x, level_y, 0.0f, glm::vec2(1.0f, 1.0f), TILE_SOLID);
                addTile(generatedTile, FRONT_LAYER);
            }
            if (noiseValue > pivotValueForTile - 0.1f) {
                Tile backgroundTile(level_x, level_y, -1.0f, glm::vec2(1.0f, 1.0f), TILE_BACKGROUND);
                addTile(backgroundTile, BACK_LAYER);
            }
        }
    }

    tileModels.emplace_back("assets/models/gate/base.obj");
    Model *gateModel = &tileModels.back();
    Tile gate(2.0f, 2.0f, -1.0f, glm::vec2(1.0f, 1.0f), TILE_BACKGROUND, gateModel);
    addTile(gate);
}
void Level::render(Renderer *renderer, Camera *camera) {
    renderBackground(renderer, &shapeShader, camera);

    std::vector<glm::mat4> groundTileModels;
    addTileModels(&groundTileModels, camera);

    renderInstancedTiles(renderer, camera, &groundTileModels);
}

void Level::addTileModels(std::vector<glm::mat4> *groundTileModels, Camera *camera) {
    std::set<std::pair<int, int>> renderedChunks = chunksToRender(camera);
    groundTileModels->reserve(renderedChunks.size() * CHUNK_SIZE * CHUNK_SIZE * 2);

    for (auto& chunk: renderedChunks) {
        auto frontChunkTileIt = frontLayerChunks.find(chunk);
        if (frontChunkTileIt == frontLayerChunks.end()) {
            continue;
        }
        for (auto& tilePair: frontChunkTileIt->second) {
            Tile& tile = tilePair.second;
            if (tile.hasModel()) {
                tile.render(&modelShader);
            } else {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(tile.getPositionX(), tile.getPositionY(), tile.getPositionZ()));
                groundTileModels->push_back(model);
            }
        }
        auto backChunkTileIt = backLayerChunks.find(chunk);
        if (backChunkTileIt == backLayerChunks.end()) {
            continue;
        }
        for (auto& tilePair: backChunkTileIt->second) {
            Tile& tile = tilePair.second;
            if (tile.hasModel()) {
                tile.render(&modelShader);
            } else {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(tile.getPositionX(), tile.getPositionY(), tile.getPositionZ()));
                groundTileModels->push_back(model);
            }
        }
    }
}

void Level::update() {
    player->setOnGround(false);
    checkCollisions(player);
    player->checkMapBounds(leftBound, rightBound, bottomBound, topBound);
}

void Level::checkCollisions(Entity *entity) {
    AABB entityAABB = entity->getAABB();

    std::set<std::pair<int, int>> nearbyChuncks;
    nearbyChuncks.insert(getChunkCoordinates(entityAABB.minX, entityAABB.minY));
    nearbyChuncks.insert(getChunkCoordinates(entityAABB.minX, ceil(entityAABB.maxY)));
    nearbyChuncks.insert(getChunkCoordinates(entityAABB.maxX, entityAABB.minY));
    nearbyChuncks.insert(getChunkCoordinates(entityAABB.maxX, ceil(entityAABB.maxY)));
    nearbyChuncks.insert(getChunkCoordinates(ceil(entityAABB.maxX), entityAABB.minY));
    nearbyChuncks.insert(getChunkCoordinates(ceil(entityAABB.maxX), entityAABB.maxY));

    for (auto& chunk: nearbyChuncks) {
        auto chunkTileIt = frontLayerChunks.find(chunk);
        if (chunkTileIt == frontLayerChunks.end()) {
            continue;
        }
        for (auto& tilePair: chunkTileIt->second) {
            Tile& tile = tilePair.second;
            if (tile.getAABB().intersects(entityAABB)) {
                entity->handleCollision(&tile);
            }
        }
    }
}

bool Level::chunkInsideBounds(int chunkX, int chunkY) {
    return (chunkX >= floor(leftBound / CHUNK_SIZE) &&
            chunkX <= floor(rightBound / CHUNK_SIZE) &&
            chunkY >= floor(bottomBound / CHUNK_SIZE) &&
            chunkY <= floor(topBound / CHUNK_SIZE));
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

void Level::renderInstancedTiles(Renderer *renderer, Camera *camera, std::vector<glm::mat4> *groundTileModels) {
    if (!groundTileModels->empty()) {
        groundShader.use();

        groundShader.setMat4("view", camera->getViewMatrix());
        groundShader.setMat4("projection", camera->getProjectionMatrix());
        groundShader.setVec3("viewPos", camera->getPosition());

        groundShader.setInt("material.diffuse", groundTexture.getUnit());
        groundShader.setInt("material.specular", groundSpecularTexture.getUnit());
        groundTexture.bind();
        groundSpecularTexture.bind();

        groundShader.setDirLight(glm::vec3(0.2f, -0.3f, -1.0f), glm::vec3(0.05f), glm::vec3(0.5f), glm::vec3(0.5f));
        groundShader.setPointLight(0, player->getPosition() + glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.01f), glm::vec3(0.95f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f));
        groundShader.setSpotLight(player->getPosition() + glm::vec3(-3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f, 0.09f, 0.032f), glm::vec2(glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(15.0f))));
        groundShader.setFloat("shininess", 0.5f * 128.0f);

        renderer->updateInstanceBuffer(*groundTileModels);
        renderer->drawCubesInstanced(groundTileModels->size());
    }
}


void Level::renderBackground(Renderer* renderer, Shader *shader, Camera *camera) {
    shader->use();

    shader->setInt("material.diffuse", backgroundTexture.getUnit());
    shader->setInt("material.specular", backgroundSpecularTexture.getUnit());
    backgroundTexture.bind();
    backgroundSpecularTexture.bind();

    shader->setDirLight(glm::vec3(0.0f, 0.0f, -1.0f),
                        glm::vec3(0.05f),
                        glm::vec3(0.7f),
                        glm::vec3(0.0f));
    shader->setPointLight(0, glm::vec3(0.0f), glm::vec3(0.00f), glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f, 0.09f, 0.032f));
    shader->setSpotLight(player->getPosition(), 
                        glm::normalize(player->getPosition() - camera->getPosition()),
                        glm::vec3(0.0f),
                        glm::vec3(0.0f), // spotlight disabled
                        glm::vec3(1.0f),
                        glm::vec3(1.0f, 0.0f, 0.0f),
                        glm::vec2(glm::cos(glm::radians(8.0f)), glm::cos(glm::radians(20.0f))));

    renderer->drawSquare(*shader, glm::vec3(0.0f, 255.0f, -100.0f), glm::vec3(630.0f, 680.0f, 1.0f));
}