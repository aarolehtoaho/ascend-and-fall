#include "Perlin2D.h"

Perlin2D::Perlin2D(unsigned int width, unsigned int height, unsigned int seed) {
    this->width = width;
    this->height = height;
    vectorMap.resize((width + 1) * (height + 1));

    srand(seed);
    for (unsigned int y = 0; y <= height; y++) {
        for (unsigned int x = 0; x <= width; x++) {
            int random_x = rand();
            random_x = random_x % 2 == 0 ? random_x : -random_x;
            int random_y = rand();
            random_y = random_y % 2 == 0 ? random_y : -random_y;
            glm::vec2 gradientVector = glm::vec2(random_x, random_y);
            gradientVector = glm::normalize(gradientVector);
            setVector(x, y, gradientVector);
        }
    }
}

bool Perlin2D::setVector(unsigned int x, unsigned int y, const glm::vec2& vector) {
    if (x > width || y > height) {
        return false;
    }
    vectorMap[y * (width + 1) + x] = vector;
    return true;
}

glm::vec2 Perlin2D::getVector(unsigned int x, unsigned int y) const {
    if (x > width || y > height) {
        return glm::vec2(0.0f, 0.0f);
    }
    return vectorMap[y * (width + 1) + x];
}

float Perlin2D::getNoise(float x, float y) const {
    glm::vec2 v00 = getVector(glm::floor(x), glm::floor(y));
    glm::vec2 v10 = getVector(glm::ceil(x), glm::floor(y));
    glm::vec2 v01 = getVector(glm::floor(x), glm::ceil(y));
    glm::vec2 v11 = getVector(glm::ceil(x), glm::ceil(y));

    float dot00 = glm::dot(v00, glm::vec2(x - glm::floor(x), y - glm::floor(y)));
    float dot10 = glm::dot(v10, glm::vec2(x - glm::ceil(x), y - glm::floor(y)));
    float dot01 = glm::dot(v01, glm::vec2(x - glm::floor(x), y - glm::ceil(y)));
    float dot11 = glm::dot(v11, glm::vec2(x - glm::ceil(x), y - glm::ceil(y)));

    float ix0 = interpolate(dot00, dot10, x - glm::floor(x));
    float ix1 = interpolate(dot01, dot11, x - glm::floor(x));
    float noiseValue = interpolate(ix0, ix1, y - glm::floor(y));

    return noiseValue;
}

float Perlin2D::interpolate(float a, float b, float t) const {
    float fade = t * t * t * (t * (t * 6 - 15) + 10);
    return a + fade * (b - a);
}