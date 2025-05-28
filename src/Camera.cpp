#include "Camera.h"
#include "Window.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), \
                                     MovementSpeed(SPEED), \
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

void Camera::processKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;

    glm::vec3 dirVec;

    switch (direction) {
        case FORWARD:
            dirVec = Front;
            break;
        case BACKWARD:
            dirVec = -Front;
            break;
        case LEFT:
            dirVec = -Right;
            break;
        case RIGHT:
            dirVec = Right;
            break;
        case UP:
            dirVec = Up;
            break;
        case DOWN:
            dirVec = -Up;
            break;
    }

    glm::vec3 transform = glm::normalize(dirVec) * velocity;

    //if (transform.y < 0.0f)
    //    transform.y = 0.0f;

    Position += transform;
}
void Camera::processMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 60.0f)
        Zoom = 60.0f;
}