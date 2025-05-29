#include "Entity.h"

Entity::Entity(glm::vec3 pos) : position(pos) {
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);
    mass = 10.0f;
    movementSpeed = 1.5f;
    health = 100.0f;
    attackPower = 10.0f;
}

void Entity::applyForce(glm::vec3 force) {
    acceleration += (50.0f * force) / mass;
}
void Entity::applyResistanceForces(float deltaTime) {
    float resistanceMagnitude = mass * deltaTime;
    velocity -= velocity * resistanceMagnitude;
}
void Entity::update(float deltaTime) {
    velocity += acceleration * deltaTime;
    if (velocity.x > movementSpeed) {
        velocity.x = movementSpeed;
    } else if (velocity.x < -movementSpeed) {
        velocity.x = -movementSpeed;
    }

    position += velocity * deltaTime;

    if (acceleration.x == 0.0f || (velocity.x * acceleration.x < 0.0f)) {
        applyResistanceForces(deltaTime);
    }

    acceleration = glm::vec3(0.0f);
}

const float PHYSICAL = 0;
const float POISON = 0;
const float ICE = 0;
const float FIRE = 0;
const float SOUL = 0;