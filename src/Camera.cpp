#include "Camera.h"
#include "Window.h"
#include "Utils.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), \
                                     Zoom(ZOOM), \
                                     Position(position), \
                                     Up(glm::vec3(0.0f, 1.0f, 0.0f)), \
                                     Right(glm::normalize(glm::cross(Front, Up))) {}
glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}
glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(Zoom), Window::getAspectRatio(), 0.1f, 120.0f);
}

void Camera::processMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 20.0f)
        Zoom = 20.0f;
    if (Zoom > 80.0f)
        Zoom = 80.0f;
}
void Camera::moveCamera(glm::vec3 targetPosition, float speed) {
    if (targetPosition.y - distanceToBottomEdge() < 0.0f) { // Change magic numbers to world dimension constants
        targetPosition.y = distanceToBottomEdge();
    } else if (targetPosition.y + distanceToBottomEdge() > 512.0f) {
        targetPosition.y = 512.0f - distanceToBottomEdge();
    }
    if (targetPosition.x - distanceToSideEdge() < -123.0f) {
        targetPosition.x = -123.0f + distanceToSideEdge();
    } else if (targetPosition.x + distanceToSideEdge() > 123.0f) {
        targetPosition.x = 123.0f - distanceToSideEdge();
    }

    glm::vec3 direction = (targetPosition - Position); // No normalization for acceleration based on distance
    direction.z = 0.0f;

    float deltaTime = Utils::getDeltaTime();
    Position += direction * speed * deltaTime;
}
float Camera::distanceToBottomEdge() {
    return getCameraDistance() * tan(glm::radians(Zoom) / 2.0f);
}
float Camera::distanceToSideEdge() {
    return distanceToBottomEdge() * Window::getAspectRatio();
}