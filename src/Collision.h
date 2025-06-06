#ifndef COLLISON_H
#define COLLISON_H

#include <glm/glm.hpp>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
    
    bool intersects(const AABB& entity) const {
        return (min.x <= entity.max.x && max.x >= entity.min.x) &&
               (min.y <= entity.max.y && max.y >= entity.min.y) /* &&
               (min.z <= entity.max.z && max.z >= entity.min.z) */;
    }
};

#endif