#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "Shader.h"

#define MAX_BONE_INFLUENCE 4
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};
struct MeshTexture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture>  textures;
    unsigned int VAO;

    Mesh (std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);

    void draw(Shader &shader);
private:
    unsigned int VBO, EBO;

    void setupMesh();
};
#endif