#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"
#include "Shader.h"

class Model;
class Player : public Entity {
private:
    Shader *shader;
public:
    Player(glm::vec3 position, Model *model, Shader *shader);
    void draw();
};
#endif