#ifndef LEVEL_H
#define LEVEL_H

#include "Texture.h"
#include "Shader.h"
#include "Tile.h"

#include <vector>
#include <unordered_map>
#include <set>

enum levelName {
    FOREST,
    POISONOUS_HOLLOW,
    SCORCHING_DUNES,
    NIHMIRS_TOMB,
    MISTY_CAVES,
    FROST_BREACH,
    MOLTEN_DEPTHS,
    CRYPT_OF_AETHEROS,
    AETHEROS
};
struct PairHash {
    std::size_t operator()(const std::pair<int, int>& p) const {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

extern const int CHUNK_SIZE;

class Entity;
class Logger;
class Renderer;
class Model;
class Camera;
class Player;

class Level {
private:
    levelName name;

    float leftBound;
    float rightBound;
    float bottomBound;
    float topBound;

    Shader modelShader;
    Shader shapeShader;
    Texture backgroundTexture;
    Texture backgroundSpecularTexture;
    std::vector<Model> tileModels;
    std::vector<Model> entityModels;

    Player *player;

    std::unordered_map<std::pair<int, int>, std::vector<Entity>, PairHash> chunkEntities;
    std::unordered_map<std::pair<int, int>, std::vector<Tile>, PairHash> chunkTiles;
    
    void addTile(Tile tile);
    void addEntity(Entity entity);

    std::pair<int, int> getChunkCoordinates(float posX, float posY) const;
    std::set<std::pair<int, int>> chunksToRender(Camera *camera, float playerPositionX, float playerPositionY);
    bool chunkInsideBounds(int chunkX, int chunkY);

    void createForest();
    void renderBackground(Renderer *renderer, Shader *shader);

    static Logger logger;
public:
    Level(levelName name, Player *player);
    levelName getName() const { return name; }

    void render(Renderer *renderer, Camera *camera, glm::vec3 playerPosition);
    void update();
    void checkCollisions(Entity *entity);
};

#endif