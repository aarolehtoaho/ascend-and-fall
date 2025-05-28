#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "Texture.h"

Renderer::Renderer(Camera& camera) {
    this->camera = &camera;
    init();
};

void Renderer::draw(Shader& shader, \
              glm::vec3 position, \
              glm::vec3 scale, \
              glm::vec3 rotation, \
              float angle) {
    shader.setVec3("viewPos", camera->getPosition());
    shader.setFloat("material.shininess", 0.5f * 128.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(angle), rotation);
    model = glm::scale(model, scale);
    shader.setMat4("model", model);

    glm::mat4 view = camera->getViewMatrix();
    shader.setMat4("view", view);

    glm::mat4 projection = camera->getProjectionMatrix();
    shader.setMat4("projection", projection);

    // Compute normal matrix on CPU and send to shader
    glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
    shader.setMat3("normalMatrix", normalMatrix);
};

void Renderer::drawSquare(Shader& shader, \
                          glm::vec3 position, \
                          glm::vec3 scale, \
                          glm::vec3 rotation, \
                          float angle) {
    shader.use();
    draw(shader, position, scale, rotation, angle);
    glBindVertexArray(VAO_S);
    glDrawArrays(GL_TRIANGLES, 0, 6);
};
void Renderer::drawCube(Shader& shader, \
                        glm::vec3 position, \
                        glm::vec3 scale, \
                        glm::vec3 rotation, \
                        float angle) {
    shader.use();
    draw(shader, position, scale, rotation, angle);
    glBindVertexArray(VAO_C);
    glDrawArrays(GL_TRIANGLES, 0, 36);
};

void Renderer::init() {
    glGenVertexArrays(1, &VAO_S);
    glGenVertexArrays(1, &VAO_C);
    glGenBuffers(1, &VBO_S);
    glGenBuffers(1, &VBO_C);

    // square
    glBindVertexArray(VAO_S);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_S);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 8, SQUARE_VERTICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // cube
    glBindVertexArray(VAO_C);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_C);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36 * 8, CUBE_VERTICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
};

const float Renderer::SQUARE_VERTICES[] = {
    // positions         // texcoords      // normals
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f,  // left bottom
     0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  // right bottom
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,  // right top
     0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,  // right top
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f,   0.0f, 0.0f, 1.0f,  // left top
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, 1.0f   // left bottom
};
const float Renderer::CUBE_VERTICES[] = {
    // positions         // texcoords      // normals
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f, // bottom-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  0.0f, -1.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   0.0f,  0.0f, -1.0f, // bottom-right         
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  0.0f, -1.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f,  0.0f, -1.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  0.0f, -1.0f, // top-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  0.0f, 1.0f,  // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  0.0f, 1.0f,  // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f,  0.0f, 1.0f,  // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f,  0.0f, 1.0f,  // top-right
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0.0f,  0.0f, 1.0f,  // top-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  0.0f, 1.0f,  // bottom-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f, // top-right
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f,  0.0f,  0.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f,  0.0f,  0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f,  0.0f,  0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f,  0.0f,  0.0f, // top-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1.0f,  0.0f,  0.0f, // top-left
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f,  0.0f,  0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1.0f,  0.0f,  0.0f, // top-right         
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f,  0.0f,  0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1.0f,  0.0f,  0.0f, // top-left
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1.0f,  0.0f,  0.0f, // bottom-left     
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f,  0.0f, // top-right
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   0.0f, -1.0f,  0.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f,  0.0f, // bottom-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f,  0.0f, // bottom-left
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, -1.0f,  0.0f, // bottom-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f,  0.0f, // top-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  1.0f,  0.0f, // top-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f,  1.0f,  0.0f, // top-right     
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f,  1.0f,  0.0f, // bottom-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f,  1.0f,  0.0f, // top-left
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0.0f,  1.0f,  0.0f  // bottom-left
};

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO_S);
    glDeleteVertexArrays(1, &VAO_C);
    glDeleteBuffers(1, &VBO_S);
    glDeleteBuffers(1, &VBO_C);
}