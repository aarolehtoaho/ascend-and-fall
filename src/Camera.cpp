#include "Camera.h"
#include "Window.h"

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
    return glm::perspective(glm::radians(Zoom), Window::getAspectRatio(), 0.1f, 100.0f);
}

void Camera::processMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 10.0f)
        Zoom = 10.0f;
    if (Zoom > 60.0f)
        Zoom = 60.0f;
}
void Camera::moveCamera(glm::vec3 targetPosition, float speed, float deltaTime) {
    glm::vec3 direction = (targetPosition - Position); // No normalization for acceleration based on distance
    direction.z = 0.0f;
    Position += direction * speed * deltaTime;
}