#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <ImGuiFileDialog.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <implot.h>

#include "UI.h"
#include "properties.h"

namespace fs = std::filesystem;

UI::UI(GLFWwindow* window, Config* config, Scene* scene, Camera* camera)
    : window(window), config(config), scene(scene), camera(camera) {
    // IMGUI initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Font configuration.
    // Main font.
    io.Fonts->AddFontFromFileTTF("editor_assets/RobotoMono-Regular.ttf", 18.0f);
    // Icon font.
    ImFontConfig imfc;
    imfc.MergeMode        = true;
    imfc.GlyphMaxAdvanceX = 14.0f;
    imfc.GlyphOffset.y += 5.0f;
    static const ImWchar iconRanges[] = {0xE000, 0xE0FE, 0};
    io.Fonts->AddFontFromFileTTF("editor_assets/OpenFontIcons.ttf", 22.0f, &imfc, iconRanges);
    // Build font altas.
    io.Fonts->Build();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(NULL);

    updateWindowTitle();
    for (auto& [id, _] : scene->objectsStorage) {
        this->showWindowProperties[id] = false;
    }
}

UI::~UI() {
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
}

void UI::initFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        mainMenu();
        // Main "static" windows.
        windowCamera();
        windowObjects();
        windowStats();
        windowTexturePreview();
        // Properties windows.
        windowsProperties();

        // Open scene dialog.
        dialogOpenScene();

        // Modal popup for creating a new object.
        popupNewObject();
        // Modal popup for renaming an object.
        popupRenameObject();
        // Modal popup for deleting object.
        popupDeleteObject();
    }
}

void UI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Main menu (the one under application window header).
void UI::mainMenu() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Scene", true)) {
            //            ImGui::MenuItem("New");
            if (ImGui::MenuItem("Open")) {
                showDialogOpenScene = true;
            }
            if (ImGui::MenuItem("Save")) {
                scene->save();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window", true)) {
            ImGui::Checkbox("Objects", &showWindowObjects);
            ImGui::Checkbox("Camera", &showWindowCamera);
            ImGui::Checkbox("Stats", &showWindowStats);
            ImGui::Checkbox("Texture preview", &showWindowTexturePreview);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

// Helper function to reduce window creation boilerplating.
bool UI::newImGuiWindow(const char* name, bool* show, float posX, float posY, float sizeX,
                        float sizeY, ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse) {
    // Instantly return false if the window should not be displayed.
    if (!*show) {
        return false;
    }

    ImGui::SetNextWindowPos(ImVec2(posX, posY), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(sizeX, sizeY), ImGuiCond_FirstUseEver);
    return ImGui::Begin(name, show, flags);
}

// Window to display rendering stats.
void UI::windowStats() {
    if (newImGuiWindow("Stats", &showWindowStats, 0, 500, 400, 220)) {
        ImGui::Text("Frame time graph:");
        ImPlot::SetNextPlotLimitsX(0, 300.0f, ImGuiCond_Appearing);
        ImPlot::SetNextPlotLimitsY(0, scene->stats->frameTimeHistory.Max, ImGuiCond_Always);
        if (ImPlot::BeginPlot("##Frametimes", NULL, NULL, ImVec2(-1, 120), 0,
                              ImPlotAxisFlags_NoTickLabels | ImPlotAxisFlags_Lock)) {
            ImPlot::PlotLine<double>("", &scene->stats->frameTimeHistory.Data[0],
                                     scene->stats->frameTimeHistory.Data.size(), 1, 0,
                                     scene->stats->frameTimeHistory.Offset);
            ImPlot::EndPlot();
        }
        ImGui::Text("FPS: %d", scene->stats->fps);
        ImGui::Text("Draw calls: %d", scene->stats->drawCalls);
        ImGui::End();
    }
}

// Camera control window.
void UI::windowCamera() {
    if (newImGuiWindow("Camera", &showWindowCamera, 980, 26, 320, 180)) {
        if (ImGui::CollapsingHeader("Coordinates")) {
            ImGui::DragFloat3("position", (float*)&camera->position, 0.05f, -20.0f, 20.0f, "%.2f");
        }
        ImGui::Dummy(ImVec2(0, 5.0f));
        ImGui::Text("Move camera with:");
        ImGui::Checkbox("WASD on keyboard", &camera->allowKeyboardControl);
        ImGui::Checkbox("right mouse button dragging", &camera->allowMouseControl);
        ImGui::End();
    }
}

// Objects management window.
void UI::windowObjects() {
    if (newImGuiWindow("Objects", &showWindowObjects, 0, 26, 300, 400,
                       ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar)) {
        // Menu
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("Add")) {
                // Generate menu entries for different object types.
                for (auto& [type, name] : Scene::listObjectTypes()) {
                    if (ImGui::MenuItem(name.c_str())) {
                        newObjectType      = type;
                        newObjectTypeName  = name;
                        showPopupNewObject = true;
                    }
                }
                ImGui::Separator();
                for (auto& [type, name] : Drawable::listObjectTypes()) {
                    if (ImGui::MenuItem(name.c_str())) {
                        newObjectType      = type;
                        newObjectTypeName  = name;
                        showPopupNewObject = true;
                    }
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Objects list
        for (auto& id : scene->sortedObjectIDs) {
            auto displayName = scene->getObjectDisplayName(id);

            if (ImGui::Selectable(displayName.c_str(), false,
                                  ImGuiSelectableFlags_AllowDoubleClick)) {

                if (ImGui::IsMouseDoubleClicked(0)) {
                    showWindowProperties[id] = true;
                }
            }
            if (ImGui::BeginPopupContextItem()) {
                ImGui::Text("%s", displayName.c_str());
                ImGui::Separator();
                if (ImGui::Selectable("Properties", false)) {
                    showWindowProperties[id] = true;
                }
                // Special entry for drawables only.
                auto obj = scene->getObjectByID<Drawable>(id);
                if (obj && ImGui::Selectable("Move camera here", false)) {
                    camera->go(obj->position);
                }
                if (ImGui::Selectable("Rename", false)) {
                    currentObjectID       = id;
                    showPopupRenameObject = true;
                }
                if (ImGui::Selectable("Delete", false)) {
                    currentObjectID       = id;
                    showPopupDeleteObject = true;
                }
                ImGui::EndPopup();
            }
        }

        ImGui::End();
    }
}

// Texture previewing window, used mostly for debugging purposes.
void UI::windowTexturePreview() {
    if (newImGuiWindow("Texture preview", &showWindowTexturePreview, 50, 50, 600, 600)) {
        ImGui::Text("Texture ID:");
        ImGui::SameLine();
        ImGui::PushItemWidth(120);

        if (ImGui::BeginCombo("##textureID", std::to_string(selectedTexture).c_str())) {
            for (unsigned int id = 0; id < 8; id++) {
                bool is_selected;
                is_selected = selectedTexture && (selectedTexture == id);
                if (ImGui::Selectable(std::to_string(id).c_str(), is_selected)) {
                    selectedTexture = id;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::PopItemWidth();

        // Draw the texture but keep proportions and fit it inside the window.
        // Assume that all the textures we care of here are square ones.
        ImVec2 wSize = ImGui::GetContentRegionAvail();
        if (wSize.x < wSize.y) {
            wSize.y = wSize.x;
        } else {
            wSize.x = wSize.y;
        }
        ImGui::Image((void*)(intptr_t)selectedTexture, wSize);

        ImGui::End();
    }
}

void UI::windowsProperties() {
    for (auto& [id, o] : scene->objectsStorage) {
        if (!showWindowProperties[id]) {
            continue;
        }

        ImGui::SetNextWindowSize(ImVec2(350, 300), ImGuiCond_FirstUseEver);
        char windowTitle[256] = "properties";
        sprintf(windowTitle, "%s - properties", scene->getObjectDisplayName(id).c_str());
        if (ImGui::Begin(windowTitle, &showWindowProperties[id], ImGuiWindowFlags_NoCollapse)) {
            properties(id, o->type, scene);
            ImGui::End();
        }
    }
}

// Helper function to create open file dialog windows.
std::string UI::dialogOpenFile(const char* key, const char* title, bool* show,
                               const char* filters) {
    if (!*show) {
        return "";
    }

    // Display configuration.
    ImGui::SetNextWindowSize(ImVec2(700, 450), ImGuiCond_FirstUseEver);

    // Filters are expected to be in space separated format ".c .cpp .h"
    // The following fragment of code converts them to the format ".c .cpp .h{.c,.cpp,.h},.*"
    // expected by ImGuiFileDialog and creates a vector for SetExtentionInfos method application.
    char titleBuf[512];
    std::vector<std::string> extensions;
    char extBuf[128] = ".";

    strcpy_s(titleBuf, sizeof(titleBuf), filters);
    unsigned int bi  = strlen(filters);
    int ei           = 1;
    titleBuf[bi]     = '{';
    titleBuf[bi + 1] = '.';
    bi++;
    char ch;
    do {
        ch = *++filters;
        bi++;

        if (ch != ' ' && ch != '\0') {
            titleBuf[bi] = ch;
            extBuf[ei]   = ch;
        } else {
            titleBuf[bi] = ',';
            extBuf[ei]   = '\0';
            extensions.emplace_back(extBuf);
            ei = -1;
        }
        ei++;
    } while (ch != '\0');

    bi++;
    const char* tail = "},.*";
    for (char cc = *tail; cc; cc = *++tail) {
        titleBuf[bi] = cc;
        bi++;
    }
    titleBuf[bi] = '\0';

    // Instantiate.
    ImGuiFileDialog::Instance()->OpenDialog(key, title, titleBuf, ".");

    for (std::string& e : extensions) {
        ImGuiFileDialog::Instance()->SetExtentionInfos(e, ImVec4(0, 1, 0.5, 1));
    }

    // Display.
    std::string filePathName;
    if (ImGuiFileDialog::Instance()->Display(key)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
        }

        // Close.
        ImGuiFileDialog::Instance()->Close();
        *show = false;
    }

    return filePathName;
}

// Scene file selection dialog.
void UI::dialogOpenScene() {
    std::string file =
        dialogOpenFile("OpenScene", "Open scene file", &showDialogOpenScene, ".scene");
    if (!file.empty()) {
        scene->loadFromFile(file.c_str());
        config->saveSceneFile(file);
        updateWindowTitle();
    }
}

void UI::popupNewObject() {
    if (showPopupNewObject) {
        ImGui::OpenPopup("New object");
        showPopupNewObject = false;
    }
    if (ImGui::BeginPopupModal("New object", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Object type: %s", newObjectTypeName.c_str());
        static char name[256];
        ImGui::Text("Object name:");
        ImGui::SameLine();
        ImGui::InputText("", name, IM_ARRAYSIZE(name));
        if (ImGui::Button("Create")) {
            ObjectID newId              = scene->createObject(newObjectType, name);
            showWindowProperties[newId] = false;
            name[0]                     = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            name[0] = '\0';
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void UI::popupRenameObject() {
    static char newName[256];

    if (showPopupRenameObject) {
        ImGui::OpenPopup("Rename object");
        showPopupRenameObject = false;

        // This block is here to avoid copying and reinstantiation of newName on each frame, it
        // should happen only when the popup appears.
        const std::string& name = scene->getBaseObjectByID(currentObjectID)->name;
        name.copy(newName, name.size(), 0);
        newName[name.size()] = '\0';
    }
    if (ImGui::BeginPopupModal("Rename object", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        // TODO: display object type when renaming.
        //        ImGui::Text("Object type: %s", newObjectTypeName.c_str());
        ImGui::Text("Object name:");
        ImGui::SameLine();
        ImGui::InputText("##name", newName, 256);
        if (ImGui::Button("Rename")) {
            scene->renameObject(currentObjectID, std::string(newName));
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void UI::popupDeleteObject() {
    if (showPopupDeleteObject) {
        ImGui::OpenPopup("Delete object");
        showPopupDeleteObject = false;
    }

    if (ImGui::BeginPopupModal("Delete object", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure about deletion of '%s'?",
                    scene->getObjectDisplayName(currentObjectID).c_str());
        std::vector<ISceneObject*> ancesors = scene->getAncestors(currentObjectID);
        ImGui::Dummy(ImVec2(0.0f, 10.0f));
        if (!ancesors.empty()) {
            ImGui::Text(
                "These objects have links to this one, after deletion they may work incorrectly:");
            for (auto anc : ancesors) {
                ImGui::BulletText("%s %s", anc->icon(), anc->name.c_str());
            }
            ImGui::Dummy(ImVec2(0.0f, 10.0f));
        }
        // Center Yes/No buttons.
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - 102);
        if (ImGui::Button("Yes", ImVec2(100.0f, 25.0f))) {
            scene->deleteObject(currentObjectID);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("No", ImVec2(100.0f, 25.0f))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void UI::updateWindowTitle() {
    char newTitle[256];
    std::string basename = config->sceneFile.substr(config->sceneFile.find_last_of("/\\") + 1);
    sprintf(newTitle, "%s - Hazard", basename.c_str());
    glfwSetWindowTitle(window, newTitle);
}
