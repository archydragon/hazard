#ifndef HAZARD_UI_H
#define HAZARD_UI_H

#include "../engine/Camera.h"
#include "../engine/Scene.h"
#include <imgui.h>

class UI {
public:
    explicit UI(GLFWwindow* window, Scene* pCamera, Camera* pScene);
    void initFrame();
    static void render();
    virtual ~UI();

    static bool cameraFreelook;

private:
    ImGuiIO io;
    Camera* camera;
    Scene* scene;
    bool windowCamera = true;
};

#endif // HAZARD_UI_H
