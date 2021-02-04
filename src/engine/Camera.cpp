#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

using namespace glm;

Camera::Camera(vec3 initPosition, vec3 initFront) {
    position = initPosition;
    front    = initFront;
    // Set default values.
    worldUp         = vec3(0.0f, 1.0f, 0.0f);
    yaw             = -90.0f;
    pitch           = 0.0f;
    movementSpeed   = 0.3f;
    lookSensitivity = 0.1f;
    updateCameraVectors();
}

mat4 Camera::getViewMatrix() {
    return lookAt(position, position + front, up);
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
