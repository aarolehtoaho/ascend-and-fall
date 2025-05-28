#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "Logger.h"

class Shader {
private:
    unsigned int ID;
    int getUniformLocation(const std::string &name) const;
    void checkCompileErrors(unsigned int shader, std::string type);
    static Logger logger;
public:
    Shader(std::string vertexPath, std::string fragmentPath);
    ~Shader();

    void use();
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, glm::vec3 vect) const;
    void setVec4(const std::string &name, glm::vec4 vect) const;
    void setMat3(const std::string &name, glm::mat3 matrix) const;
    void setMat4(const std::string &name, glm::mat4 matrix) const;

    void setDirLight(glm::vec3 direction, \
                     glm::vec3 ambient, \
                     glm::vec3 diffuse, \
                     glm::vec3 specular) const;
    void setPointLight( int index, \
                        glm::vec3 position, \
                        glm::vec3 ambient, \
                        glm::vec3 diffuse, \
                        glm::vec3 specular, \
                        glm::vec3 attenuation) const;
    void setSpotLight(glm::vec3 position, \
                      glm::vec3 direction, \
                      glm::vec3 ambient, \
                      glm::vec3 diffuse, \
                      glm::vec3 specular, \
                      glm::vec3 attenuation, \
                      glm::vec2 cutOffs) const;
};
#endif