#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

const float ZOOM  = 70.0f;

class Camera {
private:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;

    float Zoom;

    Camera();


public:
    Camera(glm::vec3 position);
    glm::vec3 getPosition() { return Position; }
    glm::vec3 getFront() { return Front; }
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void processMouseScroll(float yoffset);

    void moveCamera(glm::vec3 targetPosition, float speed);

    float getCameraDistance() { return Position.z;}
    float distanceToBottomEdge();
    float distanceToSideEdge();
};
#endif