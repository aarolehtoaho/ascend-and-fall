#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"

int Texture::units = 0;
Logger Texture::logger("debug.log");

Texture::Texture(const std::string& filePath) {
    unit = units++;

    GLint maxUnitsGL = 0;
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnitsGL);
    if (unit > maxUnitsGL) {
        logger.log("Maximum texture units exceeded.");
        return;
    }

    glGenTextures(1, &textureID);

    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
    if (data) {
        GLenum format = GL_RGB;

        if (channels == 1) {
            format = GL_RED;
        } else if (channels == 3) {
            format = GL_RGB;
        } else if (channels == 4) {
            format = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        logger.log("Failed to load texture: " + filePath);
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
};
Texture::~Texture() {
    glDeleteTextures(1, &textureID);
};
void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, textureID);
};
void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
};
unsigned int Texture::getID() const {
    return textureID;
};
int Texture::getUnit() const {
    return unit;
};