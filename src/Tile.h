#ifndef TILE_H
#define TILE_H

#include <glm/glm.hpp>
#include <vector>

class Model;
class Shader;

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

    int tileID;
    int positionX;
    int positionY;
    glm::vec2 size;
    TileType type;

    Model* model;

    glm::vec3 getDrawingPosition();
    void setLights(Shader *shader);
public:
    Tile(int posX, int posY, glm::vec2 size, TileType type, Model* model);

    int getTileID() const { return tileID; }

    void render(Shader *shader);
};

#endif