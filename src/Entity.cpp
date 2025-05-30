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
    // Used for continuous forces to apply acceleration.
    // Acceleration is reset to zero after each update.
    acceleration += (FORCE_ADJUSTMENT * force) / mass;
}
void Entity::applyImpulse(glm::vec3 impulse) {
    // Used for instantaneous forces, such as a jump or a hit.
    // Impulse is applied directly to the velocity.
    velocity += (FORCE_ADJUSTMENT * impulse) / mass;
}
void Entity::applyFriction(float deltaTime) {
    float resistanceMagnitude = mass * deltaTime;
    velocity.x -= velocity.x * resistanceMagnitude;
}
void Entity::applyGravity(float deltaTime) {
    applyForce(GRAVITY * mass * deltaTime);
}
void Entity::applyGravityInRope(float deltaTime, glm::vec3 ropeDirection) {
    // Gravity component perpendicular to the rope direction
    // This is applied when hanging from a rope
    bool aboveRopeAttachment = ropeDirection.y >= 0.0f;
    if (aboveRopeAttachment) {
        // Gravity acts normally above the rope attachment point
        applyGravity(deltaTime);
        return;
    }

    glm::vec3 gravityComponent = glm::vec3( -GRAVITY.y * ropeDirection.x * ropeDirection.y, \
                                            -GRAVITY.y * (ropeDirection.y * ropeDirection.y - 1), \
                                            0.0f );                        
    applyForce(gravityComponent * mass * deltaTime);
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

    bool shouldApplyFriction = acceleration.x == 0.0f || (velocity.x * acceleration.x < 0.0f);
    if (shouldApplyFriction) {
        applyFriction(deltaTime);
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
const float GRAVITY_ADJUSTMENT = 1.5f;
const glm::vec3 GRAVITY = glm::vec3(0.0f, -9.81f, 0.0f) * GRAVITY_ADJUSTMENT;