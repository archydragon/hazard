#ifndef HAZARD_UI_H
#define HAZARD_UI_H

#include <memory>

#include <imgui.h>

#include "../Config.h"
#include "../engine/Camera.h"
#include "../engine/Scene.h"
#include "../engine/scene_objects/Drawable.h"
#include "../engine/scene_objects/ISceneObject.h"

class UI {
public:
    explicit UI(GLFWwindow* window, Config* config, Scene* scene, Camera* camera);
    virtual ~UI();
    void initFrame();
    static void draw();

    bool cameraFreelook = false;

    static std::string dialogOpenFile(const char* key, const char* title, bool* show,
                                      const char* filters);

private:
    GLFWwindow* window;
    Config* config;
    Camera* camera;
    Scene* scene;
    ImGuiIO io;

    // UI windows and dialogs.
    void mainMenu();
    void windowObjects();
    void windowCamera();
    void windowStats();
    void windowsProperties();
    void dialogOpenScene();
    void popupNewObject();
    void popupRenameObject();
    void popupDeleteObject();

    // Windows and dialogs display states.
    bool showWindowCamera  = true;
    bool showWindowObjects = true;
    bool showWindowStats   = true;
    std::map<ObjectID, bool> showWindowProperties;
    bool showDialogOpenScene   = false;
    bool showPopupNewObject    = false;
    bool showPopupRenameObject = false;
    bool showPopupDeleteObject = false;

    int newObjectType = 0;
    std::string newObjectTypeName;
    ObjectID currentObjectID;

    void updateWindowTitle();
    static bool newImGuiWindow(const char* name, bool* p_open, float posX, float posY, float sizeX,
                               float sizeY, ImGuiWindowFlags flags);
};

#endif // HAZARD_UI_H
