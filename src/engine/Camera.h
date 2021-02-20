#ifndef HAZARD_CAMERA_H
#define HAZARD_CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm2json.h>

class Camera {
public:
    explicit Camera(glm::vec3 position = glm::vec3(-0.7f, 0.5f, 5.0f),
                    glm::vec3 front    = glm::vec3(1.0f, -1.0f, -1.5f),
                    glm::vec3 right    = glm::vec3(1, 0, 0));
    glm::mat4 getViewMatrix();
    void processKeyboardControl(GLFWwindow* window);
    void processMouseControl(GLFWwindow* window, double xpos, double ypos);
    void updateCameraVectors();
    void go(glm::vec3 coords);

    glm::vec3 position{};
    glm::vec3 front{};

    bool allowKeyboardControl = false;
    bool allowMouseControl    = false;

private:
    glm::vec3 up{};
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 right{};
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
