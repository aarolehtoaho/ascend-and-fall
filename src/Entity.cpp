#include "Entity.h"
#include "Utils.h"
#include "Tile.h"

Entity::Entity(glm::vec3 position, float height, float width, Model *model, glm::vec3 modelOffset) \
             : position(position), height(height), width(width), model(model), modelOffset(modelOffset) {
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);
    mass = 10.0f;
    movementSpeed = 6.0f;
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

void Entity::update() {
    float deltaTime = Utils::getDeltaTime();

    velocity += acceleration * deltaTime;
    if (velocity.x > movementSpeed) {
        velocity.x = movementSpeed;
    } else if (velocity.x < -movementSpeed) {
        velocity.x = -movementSpeed;
    }

    position += velocity * deltaTime;

    bool shouldApplyFriction = acceleration.x == 0.0f || (velocity.x * acceleration.x < 0.0f);
    if (shouldApplyFriction) {
        applyFriction(deltaTime);
    }

    acceleration = glm::vec3(0.0f);
    
    if (!onGround) {
        applyGravity(deltaTime);
    }

    if (velocity.x < 0.0f) {
        lookingDirection = LEFT;
    } else if (velocity.x > 0.0f) {
        lookingDirection = RIGHT;
    }
    updateRotation(deltaTime);
}

void Entity::handleCollision(Tile *tile) {
    AABB entityAABB = getAABB();
    AABB tileAABB = tile->getAABB();
    float overlapX = 0.0f;
    float overlapY = 0.0f;
    
    switch (tile->getTileType()) {
        case TILE_SOLID:
            overlapX = std::min(entityAABB.max.x, tileAABB.max.x) - std::max(entityAABB.min.x, tileAABB.min.x);
            overlapY = std::min(entityAABB.max.y, tileAABB.max.y) - std::max(entityAABB.min.y, tileAABB.min.y);

            if (overlapX < 0 || overlapY < 0) {
                // No collision
                return;
            }
            if (overlapX < overlapY) {
                // Collision on X axis
                if (entityAABB.min.x < tileAABB.min.x) {
                    position.x -= overlapX;
                } else {
                    position.x += overlapX;
                }
                velocity.x = 0.0f;
            } else {
                // Collision on Y axis
                if (entityAABB.min.y < tileAABB.min.y) {
                    // Collision from below
                    position.y -= overlapY;
                    velocity.y = 0.0f;
                } else {
                    // Landed on top
                    position.y = tileAABB.max.y + height / 2.0f - modelOffset.y;
                    velocity.y = 0.0f;
                    onGround = true;
                }
            }
            break;
        case TILE_PLATFORM:
            break;
        case TILE_BACKGROUND:
            break;
        case TILE_LIQUID:
            break;
        case TILE_TRIGGER:
            break;
    }
}

void Entity::checkMapBounds() {
    float offsetX = width / 2.0f + modelOffset.x;
    float offsetY = height / 2.0f + modelOffset.y;
    if (position.y <= 0.0f + offsetY) {
        onGround = true;
        position.y = 0.0f + offsetY;
        velocity.y = 0.0f;
    } else if (position.y > 512.0f - offsetY) {
        position.y = 512.0f - offsetY;
        velocity.y = 0.0f;
    }
    if (position.x < -123.0f + offsetX) {
        position.x = -123.0f + offsetX;
        velocity.x = 0.0f;
    } else if (position.x > 123.0f - offsetX) {
        position.x = 123.0f - offsetX;
        velocity.x = 0.0f;
    }
}

void Entity::updateRotation(float deltaTime) {
    if (lookingDirection == LEFT) {
        rotationAngle -= ROTATE_SPEED * movementSpeed * deltaTime;
    } else if (lookingDirection == RIGHT) {
        rotationAngle += ROTATE_SPEED * movementSpeed * deltaTime;
    }
    if (rotationAngle < -90.0f) {
        rotationAngle = -90.0f;
    } else if (rotationAngle > 90.0f) {
        rotationAngle = 90.0f;
    }
}

AABB Entity::getAABB() {
    aabb.min = glm::vec3(position.x - width / 2.0f, position.y - height / 2.0f, -0.5f) + modelOffset;
    aabb.max = glm::vec3(position.x + width / 2.0f, position.y + height / 2.0f, 0.5f) + modelOffset;
    return aabb;
}

void Entity::draw() {};

const float PHYSICAL = 0;
const float POISON = 0;
const float ICE = 0;
const float FIRE = 0;
const float SOUL = 0;

const float FORCE_ADJUSTMENT = 150.0f;
const float GRAVITY_ADJUSTMENT = 2.5f;
const glm::vec3 GRAVITY = glm::vec3(0.0f, -9.81f, 0.0f) * GRAVITY_ADJUSTMENT;
const float ROTATE_SPEED = 150.0f;