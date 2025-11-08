#ifndef PERLIN2D_H
#define PERLIN2D_H

#include <glm/glm.hpp>
#include <vector>

class Perlin2D {
private:
    unsigned int width;
    unsigned int height;
    std::vector<glm::vec2> vectorMap;
public:
    Perlin2D(unsigned int width, unsigned int height, unsigned int seed);
    float getNoise(float x, float y) const;
    float interpolate(float a, float b, float t) const;
    bool setVector(unsigned int x, unsigned int y, const glm::vec2& vector);
    glm::vec2 getVector(unsigned int x, unsigned int y) const;
};
#endif