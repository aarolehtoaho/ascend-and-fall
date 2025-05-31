#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <map>

#include "Mesh.h"
#include "Shader.h"
#include "Logger.h"
#include "Animdata.h"

unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

class Model {
public:
    Model(std::string const &path, bool gamma = false);
    void draw(Shader &shader, glm::vec3 position, float angle = 0.0f, glm::vec3 rotation = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
    void drawMeshes(Shader &shader);
    
    static Logger logger;

    auto& GetBoneInfoMap() { return m_BoneInfoMap; }
    int& GetBoneCount() { return m_BoneCounter; }    
private:
    std::vector<MeshTexture> textures_loaded;
    bool gammaCorrection;
    std::vector<Mesh> meshes;
    std::string directory;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<MeshTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    std::map<std::string, BoneInfo> m_BoneInfoMap;
    int m_BoneCounter = 0;
 
    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
};
#endif