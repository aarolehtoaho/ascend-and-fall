#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include "Model.h"

extern const float PHYSICAL, POISON, ICE, FIRE, SOUL;
extern const float FORCE_ADJUSTMENT;
extern const glm::vec3 GRAVITY;
extern const float GRAVITY_ADJUSTMENT;

class Shader;

enum LookingDirection {
    LEFT,
    RIGHT
};

class Entity {
private:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float mass;
    float movementSpeed;

    Model *model;

    float health;
    float attackPower;
    float resistances[5] = {PHYSICAL, POISON, ICE, FIRE, SOUL};

    float height;
    float width;

    bool jumping = false;
    bool crouching = false;
protected:
    Model* getModel() { return model; }
    LookingDirection lookingDirection = RIGHT;
public:
    Entity(glm::vec3 position, Model *model, float height, float width);
    void applyForce(glm::vec3 force);
    void applyImpulse(glm::vec3 impulse);
    void applyFriction(float deltaTime);
    void applyGravity(float deltaTime);
    void applyGravityInRope(float deltaTime, glm::vec3 ropeDirection);
    void update(float deltaTime);

    glm::vec3 getPosition() { return position; };
    float getMovementSpeed() { return movementSpeed; };
    float getHeight() { return height; };
    float getWidth() { return width; };
    bool isJumping() { return jumping; };
    bool isCrouching() { return crouching; };

    void setJumping(bool jumping) { this->jumping = jumping; }
    void setCrouching(bool crouching) { this->crouching = crouching; }

    virtual void draw() = 0;
};

#endif