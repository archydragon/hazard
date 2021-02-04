#ifndef HAZARD_CAMERA_H
#define HAZARD_CAMERA_H

#include <glm/glm.hpp>

using namespace glm;

class Camera {
public:
    explicit Camera(vec3 initPosition = vec3(-0.7f, 0.5f, 5.0f),
                    vec3 initFront    = vec3(1.0f, -1.0f, -1.5f));
    mat4 getViewMatrix();

    vec3 position{};
    vec3 front{};

private:
    void updateCameraVectors();

    vec3 up{};
    vec3 right{};
    vec3 worldUp{};
    // Euler angles.
    float yaw;
    float pitch;
    // Camera options.
    float movementSpeed;
    float lookSensitivity;
};

#endif // HAZARD_CAMERA_H
