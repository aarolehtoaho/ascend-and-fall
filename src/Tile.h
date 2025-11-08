#ifndef TILE_H
#define TILE_H

#include "Collision.h"

#include <glm/glm.hpp>
#include <vector>

class Model;
class Shader;
class Player;
class Texture;
class Renderer;

enum TileType {
    TILE_SOLID,
    TILE_PLATFORM,
    TILE_BACKGROUND,
    TILE_LIQUID,
    TILE_TRIGGER
};

class Tile {
private:
    static int tileCount;
    static Player *player;

    int tileID;
    int positionX;
    int positionY;
    glm::vec2 size;
    TileType type;
    AABB aabb;

    Model *model;
    bool modelSet = false;

    glm::vec3 getDrawingPosition();
    void setLights(Shader *shader);
public:
    Tile(int posX, int posY, glm::vec2 size, TileType type, Model *model);
    Tile(int posX, int posY, glm::vec2 size, TileType type);

    int getTileID() const { return tileID; }
    int getPositionX() const { return positionX; }
    int getPositionY() const { return positionY; }
    AABB getAABB() const { return aabb; }
    TileType getTileType() const { return type; }
    bool hasModel() const { return modelSet; }

    void render(Shader *shader);
    void render(Renderer *renderer, Shader *shader, Texture *texture, Texture *textureSpecular);

    static void setPlayer(Player *p) { player = p; }
};

#endif