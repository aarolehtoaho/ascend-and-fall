#ifndef COLLISON_H
#define COLLISON_H

struct AABB {
    double minX;
    double minY;
    double maxX;
    double maxY;
    
    bool intersects(const AABB& entity) const {
        return (minX <= entity.maxX && maxX >= entity.minX) &&
               (minY <= entity.maxY && maxY >= entity.minY);
    }
};

#endif