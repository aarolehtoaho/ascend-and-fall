#ifndef RENDERER_H
#define RENDERER_H

#include "glm/glm.hpp"

#include "Shader.h"
#include "Camera.h"

class Renderer {
private:
    static const float SQUARE_VERTICES[];
    static const float CUBE_VERTICES[];

    unsigned int VBO_S, VBO_C;
    unsigned int VAO_S, VAO_C;

    Camera* camera;

    void draw(Shader& shader, \
              glm::vec3 position, \
              glm::vec3 scale, \
              glm::vec3 rotation, \
              float angle);
    Renderer();
public:
    Renderer(Camera *camera);
    ~Renderer();
    void init();
    void drawSquare(Shader& shader, \
                    glm::vec3 position = glm::vec3(0.0f), \
                    glm::vec3 scale = glm::vec3(1.0f), \
                    glm::vec3 rotation = glm::vec3(1.0f, 0.0f, 0.0f), \
                    float angle = 0.0f);
    void drawCube(Shader& shader, \
                  glm::vec3 position = glm::vec3(0.0f), \
                  glm::vec3 scale = glm::vec3(1.0f), \
                  glm::vec3 rotation = glm::vec3(1.0f, 0.0f, 0.0f), \
                  float angle = 0.0f);
};
#endif