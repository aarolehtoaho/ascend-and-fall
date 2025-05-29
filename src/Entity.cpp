#include "Entity.h"

Entity::Entity(glm::vec3 position, float height, float width) \
             : position(position), height(height), width(width) {
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);
    mass = 10.0f;
    movementSpeed = 1.5f;
    health = 100.0f;
    attackPower = 10.0f;
}

void Entity::applyForce(glm::vec3 force) {
    acceleration += (FORCE_ADJUSTMENT * force) / mass;
}
void Entity::applyResistanceForces(float deltaTime) {
    float resistanceMagnitude = mass * deltaTime;
    velocity -= velocity * resistanceMagnitude;
}
void Entity::applyGravity(float deltaTime) {
    applyForce(GRAVITY * mass * deltaTime);
}
void Entity::update(float deltaTime) {
    velocity += acceleration * deltaTime;

    if (velocity.x > movementSpeed) {
        velocity.x = movementSpeed;
    } else if (velocity.x < -movementSpeed) {
        velocity.x = -movementSpeed;
    }

    position += velocity * deltaTime;
    if (position.y < 0.0f) {
        jumping = false;
        position.y = 0.0f;
        velocity.y = 0.0f;
    }

    if (acceleration.x == 0.0f || (velocity.x * acceleration.x < 0.0f)) {
        applyResistanceForces(deltaTime);
    }

    acceleration = glm::vec3(0.0f);
    
    applyGravity(deltaTime);
}

const float PHYSICAL = 0;
const float POISON = 0;
const float ICE = 0;
const float FIRE = 0;
const float SOUL = 0;

const float FORCE_ADJUSTMENT = 50.0f;
const float GRAVITY_ADJUSTMENT = 2.0f;
const glm::vec3 GRAVITY = glm::vec3(0.0f, -9.81f, 0.0f) * GRAVITY_ADJUSTMENT;