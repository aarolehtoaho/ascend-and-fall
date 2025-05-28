#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

const float SPEED = 2.5f;
const float ZOOM  = 45.0f;

class Camera {
private:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;

    float MovementSpeed;
    float Zoom;

    Camera();
public:
    Camera(glm::vec3 position);
    glm::vec3 getPosition() { return Position; }
    glm::vec3 getFront() { return Front; }
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void processKeyboard(Camera_Movement direction, float deltaTime);
    void processMouseScroll(float yoffset);
};
#endif