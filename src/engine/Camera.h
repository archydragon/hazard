#ifndef HAZARD_CAMERA_H
#define HAZARD_CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm2json.h>

using namespace glm;

class Camera {
public:
    explicit Camera(vec3 position = vec3(-0.7f, 0.5f, 5.0f), vec3 front = vec3(1.0f, -1.0f, -1.5f),
                    vec3 right = vec3(1, 0, 0));
    mat4 getViewMatrix();
    void processKeyboardControl(GLFWwindow* window);
    void processMouseControl(GLFWwindow* window, double xpos, double ypos);
    void updateCameraVectors();

    vec3 position{};
    vec3 front{};

    bool allowKeyboardControl = false;
    bool allowMouseControl    = false;

private:
    vec3 up{};
    vec3 worldUp = vec3(0.0f, 1.0f, 0.0f);
    vec3 right{};
    // Euler angles.
    float yaw   = -90.0f;
    float pitch = 0.0f;
    // Camera options.
    float movementSpeed   = 0.2f;
    float lookSensitivity = 0.1f;
    // Mouse control information.
    bool firstClick = true;
    float lastX     = 0;
    float lastY     = 0;

    // Macros for JSON (de)serialization.
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Camera, position, front, right, yaw, pitch)
};

#endif // HAZARD_CAMERA_H
