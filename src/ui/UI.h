#ifndef HAZARD_UI_H
#define HAZARD_UI_H

#include <deque>

#include <imgui.h>

#include "../Config.h"
#include "../engine/Camera.h"
#include "../engine/Scene.h"
#include "../engine/scene_objects/ShaderSourceFile.h"

class UI {
public:
    explicit UI(GLFWwindow* window, Config* config, Scene* pCamera, Camera* pScene);
    void initFrame();
    static void render();
    virtual ~UI();

    static bool cameraFreelook;

private:
    GLFWwindow* window;
    ImGuiIO io;
    Config* appConfig;
    Camera* camera;
    Scene* scene;
    bool windowCamera     = true;
    bool windowObjects    = true;
    bool windowOpenScene  = false;
    bool windowOpenShader = false;
    bool popupNewObject   = false;
    int newObjectType     = 0;
    std::string newObjectTypeName;
    std::deque<bool> windowProperties;

    void updateWindowTitle();
    void propertiesShaderSourceFile(std::unique_ptr<ShaderSourceFile>& optr);
};

#endif // HAZARD_UI_H
