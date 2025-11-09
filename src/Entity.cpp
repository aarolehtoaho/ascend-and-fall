#include "Entity.h"
#include "Utils.h"
#include "Tile.h"

Entity::Entity(glm::vec3 position, float height, float width, Model *model, glm::vec3 modelOffset) \
             : position(position), height(height), width(width), model(model), modelOffset(modelOffset) {
    velocity = glm::vec3(0.0f);
    acceleration = glm::vec3(0.0f);
    mass = 5.0f;
    movementSpeed = 7.0f;
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
    velocity += (IMPULSE_ADJUSTMENT * impulse) / mass;
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
    switch (tile->getTileType()) {
        case TILE_SOLID:
            handleSolidCollision(tile);
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

void Entity::handleSolidCollision(Tile *tile) {
    AABB entityAABB = getAABB();
    AABB tileAABB = tile->getAABB();    
    double overlapX = std::min(entityAABB.maxX, tileAABB.maxX) - std::max(entityAABB.minX, tileAABB.minX);
    double overlapY = std::min(entityAABB.maxY, tileAABB.maxY) - std::max(entityAABB.minY, tileAABB.minY);

    bool noCollision = overlapX <= 0 || overlapY <= 0;
    if (noCollision) {
        return;
    }

    bool collisionOnX = overlapX < overlapY;
    if (collisionOnX) {
        if (entityAABB.minX < tileAABB.minX) {
            position.x -= overlapX;
        } else {
            position.x += overlapX;
        }
        velocity.x = 0.0f;
    }
    
    bool collisionOnY = !collisionOnX;
    if (collisionOnY) {
        if (entityAABB.minY < tileAABB.minY) {
            // Headhit
            position.y -= overlapY;
            velocity.y = 0.0f;
        } else {
            // Landed on top
            position.y = tileAABB.maxY + height / 2.0f - modelOffset.y;
            velocity.y = 0.0f;
            onGround = true;
        }
    }    
}

void Entity::checkMapBounds(float left, float right, float bottom, float top) {
    float offsetX = width / 2.0f + modelOffset.x;
    float offsetY = height / 2.0f + modelOffset.y;
    if (position.y <= bottom + offsetY) {
        onGround = true;
        position.y = bottom + offsetY;
        velocity.y = 0.0f;
    } else if (position.y > top - offsetY) {
        position.y = top - offsetY;
        velocity.y = 0.0f;
    }
    if (position.x < left + offsetX) {
        position.x = left + offsetX;
        velocity.x = 0.0f;
    } else if (position.x > right - offsetX) {
        position.x = right - offsetX;
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
    aabb.minX = position.x - width / 2.0;
    aabb.minY = position.y - height / 2.0;
    aabb.maxX = position.x + width / 2.0;
    aabb.maxY = position.y + height / 2.0;
    return aabb;
}

void Entity::draw() {};

const float PHYSICAL = 0;
const float POISON = 0;
const float ICE = 0;
const float FIRE = 0;
const float SOUL = 0;

const float FORCE_ADJUSTMENT = 150.0f;
const float IMPULSE_ADJUSTMENT = 80.0f;
const float GRAVITY_ADJUSTMENT = 2.0f;
const glm::vec3 GRAVITY = glm::vec3(0.0f, -9.81f, 0.0f) * GRAVITY_ADJUSTMENT;
const float ROTATE_SPEED = 150.0f;