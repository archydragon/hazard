#ifndef HAZARD_UI_H
#define HAZARD_UI_H

#include "../Config.h"
#include "../engine/Camera.h"
#include "../engine/Scene.h"
#include <imgui.h>

class UI {
public:
    explicit UI(GLFWwindow* window, Config* config, Scene* pCamera, Camera* pScene);
    void initFrame();
    static void render();
    virtual ~UI();

    static bool cameraFreelook;

private:
    ImGuiIO io;
    Config* appConfig;
    Camera* camera;
    Scene* scene;
    bool windowCamera    = true;
    bool windowOpenScene = false;
    std::vector<const char*> files;
};

#endif // HAZARD_UI_H
