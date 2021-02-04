#ifndef HAZARD_UI_H
#define HAZARD_UI_H

#include "../engine/Camera.h"
#include <imgui.h>

class UI {
public:
    explicit UI(GLFWwindow* window, Camera* pCamera);
    void initFrame();
    static void render();
    virtual ~UI();

    static bool cameraFreelook;

private:
    ImGuiIO io;
    Camera* camera;
};

#endif // HAZARD_UI_H
