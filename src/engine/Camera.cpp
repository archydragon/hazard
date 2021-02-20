#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

using namespace glm;

Camera::Camera(vec3 position, vec3 front, vec3 right)
    : position(position), front(front), right(right) {
    updateCameraVectors();
}

mat4 Camera::getViewMatrix() {
    return lookAt(position, position + front, up);
}

void Camera::processKeyboardControl(GLFWwindow* window) {
    if (!allowKeyboardControl) {
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        position += front * movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        position -= front * movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= right * movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += right * movementSpeed;
}

void Camera::processMouseControl(GLFWwindow* window, double xpos, double ypos) {
    if (!allowMouseControl) {
        return;
    }

    if (firstClick) {
        lastX      = xpos;
        lastY      = ypos;
        firstClick = false;
    }

    float xoffset = (xpos - lastX) * lookSensitivity;
    float yoffset = (lastY - ypos) * lookSensitivity;
    lastX         = xpos;
    lastY         = ypos;

    // We don't care about mouse movements if the second mouse button is not pressed.
    if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
        return;
    }

    yaw += xoffset;
    pitch += yoffset;
    // Limit vertical look to straight up or down.
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    vec3 newFront;
    newFront.x = cos(radians(yaw)) * cos(radians(pitch));
    newFront.y = sin(radians(pitch));
    newFront.z = sin(radians(yaw)) * cos(radians(pitch));
    front      = normalize(newFront);
    // also re-calculate the Right and Up vector
    right = normalize(
        cross(front, worldUp)); // normalize the vectors, because their length gets closer to 0 the
                                // more you look up or down which results in slower movement.
    up = normalize(cross(right, front));
}
