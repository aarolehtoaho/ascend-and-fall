#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include <fstream>
#include <sstream>

Logger Shader::logger("debug.log");

Shader::Shader(std::string vertexPath, std::string fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try 
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        logger.log("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ:");
        logger.log(e.what());
    }

    const char* vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
};
Shader::~Shader() {
    glDeleteProgram(ID);
};
void Shader::use() {
    glUseProgram(ID);
};

void Shader::setBool(const std::string &name, bool value) const
{
    int location = getUniformLocation(name);
    if (location != -1)
        glUniform1i(location, (int)value);
};
void Shader::setInt(const std::string &name, int value) const
{
    int location = getUniformLocation(name);
    if (location != -1)
        glUniform1i(location, value);
};
void Shader::setFloat(const std::string &name, float value) const {
    int location = getUniformLocation(name);
    glUniform1f(location, value);
};
void Shader::setVec3(const std::string &name, glm::vec3 vect) const {
    int location = getUniformLocation(name);
    glUniform3f(location, vect.x, vect.y, vect.z);
};
void Shader::setVec4(const std::string &name, glm::vec4 vect) const {
    int location = getUniformLocation(name);
    glUniform4f(location, vect.x, vect.y, vect.z, vect.w);
};
void Shader::setMat3(const std::string &name, glm::mat3 matrix) const {
    int location = getUniformLocation(name);
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
};
void Shader::setMat4(const std::string &name, glm::mat4 matrix) const {
    int location = getUniformLocation(name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
};

void Shader::setDirLight(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diff, glm::vec3 spec) const {
    setVec3("dirLight.direction", dir);
    setVec3("dirLight.ambient", ambient);
    setVec3("dirLight.diffuse", diff);
    setVec3("dirLight.specular", spec);
}

void Shader::setPointLight(int index, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuation) const {
    std::string pointLight = "pointLights[" + std::to_string(index) + "].";
    setVec3(pointLight + "position", position);
    setVec3(pointLight + "ambient", ambient);
    setVec3(pointLight + "diffuse", diffuse);
    setVec3(pointLight + "specular", specular);
    setVec3(pointLight + "attenuation", attenuation);
    std::string pointLightSet = "pointLightSet[" + std::to_string(index) + "]";
    setBool(pointLightSet, true);
}
void Shader::setSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuation, glm::vec2 cutOffs) const {
    setVec3("spotLight.position", position);
    setVec3("spotLight.direction", direction);
    setVec3("spotLight.ambient", ambient);
    setVec3("spotLight.diffuse", diffuse);
    setVec3("spotLight.specular", specular);
    setVec3("spotLight.attenuation", attenuation);
    setFloat("spotLight.cutOff", cutOffs.x);
    setFloat("spotLight.outerCutOff", cutOffs.y);
}

int Shader::getUniformLocation(const std::string &name) const {
    int location = glGetUniformLocation(ID, name.c_str());
    if (location != -1) {
        return location;
    }
    std::string e = "Uniform not found: " + name;
    logger.log(e);
    throw std::runtime_error(e);
};

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            logger.log("ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            logger.log("ERROR::PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog + "\n");
        }
    }
};