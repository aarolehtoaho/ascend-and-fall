#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

class Player : public Entity {
private:
    Renderer *renderer;
    Shader *shader;
    Texture *textureDiffuse;
    Texture *textureSpecular;
public:
    Player(glm::vec3 position, Renderer *renderer, Shader *shader, Texture *diffuse, Texture *specular);
    void draw();
};
#endif