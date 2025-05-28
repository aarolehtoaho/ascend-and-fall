#ifndef TEXTURE_H
#define TEXTURE_H

#include "Logger.h"

#include <string>

class Texture {
private:
    static int units;
    int unit;
    unsigned int textureID;
    int width, height, channels;

    static Logger logger;
public:
    Texture(const std::string& filePath);
    ~Texture();

    void bind() const;
    void unbind() const;
    unsigned int getID() const;
    int getUnit() const;
}; 
#endif