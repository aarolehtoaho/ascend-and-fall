#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Logger.h"

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model {
public:
    Model(std::string const &path, bool gamma = false);
    void Draw(Shader &shader, glm::vec3 position, float angle = 0.0f, glm::vec3 rotation = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
    static Logger logger;
private:
    std::vector<MeshTexture> textures_loaded;
    bool gammaCorrection;
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<MeshTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

};
#endif