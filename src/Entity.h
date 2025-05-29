#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>

extern const float PHYSICAL, POISON, ICE, FIRE, SOUL;

class Entity {
    private:
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 acceleration;
        float mass;
        float movementSpeed;

        float health;
        float attackPower;
        float resistances[5] = {PHYSICAL, POISON, ICE, FIRE, SOUL};
    public:
        Entity(glm::vec3 pos);
        void applyForce(glm::vec3 force);
        void applyResistanceForces(float deltaTime);
        void update(float deltaTime);
        glm::vec3 getPosition() { return position; };
        float getMovementSpeed() { return movementSpeed; };
        virtual void draw() = 0;
};

#endif