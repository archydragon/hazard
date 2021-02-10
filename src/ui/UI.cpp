#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <ImGuiFileDialog.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "UI.h"

namespace fs = std::filesystem;

bool UI::cameraFreelook = false;

UI::UI(GLFWwindow* window, Config* config, Scene* pScene, Camera* pCamera) {
    this->window = window;

    // IMGUI initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // Font configuration.
    io.Fonts->AddFontFromFileTTF("editor_assets/RobotoMono-Regular.ttf", 18.0f);
    ImFontConfig imfc;
    imfc.MergeMode        = true;
    imfc.GlyphMaxAdvanceX = 14.0f;
    imfc.GlyphOffset.y += 5.0f;
    static const ImWchar iconRanges[] = {0xE000, 0xE0FE, 0};
    io.Fonts->AddFontFromFileTTF("editor_assets/OpenFontIcons.ttf", 22.0f, &imfc, iconRanges);
    io.Fonts->Build();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(NULL);

    appConfig = config;
    camera    = pCamera;
    scene     = pScene;

    this->updateWindowTitle();
    for (auto& [id, _] : scene->objectMap) {
        this->windowProperties[id] = false;
    }
}

UI::~UI() {
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::initFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        // Main menu.
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Scene", true)) {
                ImGui::MenuItem("New");
                if (ImGui::MenuItem("Open")) {
                    this->windowOpenScene = true;
                }
                if (ImGui::MenuItem("Save")) {
                    scene->save();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window", true)) {
                ImGui::Checkbox("Objects", &this->windowObjects);
                ImGui::Checkbox("Camera", &this->windowCamera);
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Open scene dialog.
        if (this->windowOpenScene) {
            // Instantiate.
            ImGuiFileDialog::Instance()->OpenDialog("OpenScene", "Open scene file", ".scene", ".");

            // Display configuration.
            // Set default window size.
            ImGui::SetNextWindowSize(ImVec2(700, 450), ImGuiCond_FirstUseEver);
            // Highlight .scene files with bright green.
            ImGuiFileDialog::Instance()->SetExtentionInfos(".scene", ImVec4(0, 1, 0.5, 1));

            // Display.
            if (ImGuiFileDialog::Instance()->Display("OpenScene")) {
                // action if OK
                if (ImGuiFileDialog::Instance()->IsOk()) {
                    std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    std::string filePath     = ImGuiFileDialog::Instance()->GetCurrentPath();
                    // action
                    scene->load(filePathName.c_str());
                    appConfig->saveSceneFile(filePathName);
                    this->updateWindowTitle();
                }

                // close
                ImGuiFileDialog::Instance()->Close();
                this->windowOpenScene = false;
            }
        }

        // Objects window.
        if (this->windowObjects) {
            ImGui::SetNextWindowPos(ImVec2(0, 26), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Objects", &this->windowObjects,
                             ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar)) {
                // Menu
                if (ImGui::BeginMenuBar()) {
                    if (ImGui::BeginMenu("Add")) {
                        for (auto& [type, name] : Scene::listObjectTypes()) {
                            if (ImGui::MenuItem(name.c_str())) {
                                this->newObjectType     = type;
                                this->newObjectTypeName = name;
                                this->popupNewObject    = true;
                            }
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenuBar();
                }

                // Objects list
                for (auto& [id, type] : this->scene->objectMap) {
                    char itemLine[256] = "";
                    switch (type) {
                    case SHADER_SOURCE_FILE: {
                        auto obj = this->scene->getObjectByID<ShaderSourceFile>(id);
                        sprintf(itemLine, "%s %s", obj->icon, obj->name.c_str());
                        break;
                    }
                    case SHADER_PROGRAM: {
                        auto obj = this->scene->getObjectByID<ShaderProgram>(id);
                        sprintf(itemLine, "%s %s", obj->icon, obj->name.c_str());
                        break;
                    }
                    }

                    if (ImGui::Selectable(itemLine, false, ImGuiSelectableFlags_AllowDoubleClick)) {
                        if (ImGui::IsMouseDoubleClicked(0)) {
                            this->windowProperties[id] = true;
                        }
                    }
                }

                ImGui::End();
            }
        }

        // Camera window.
        if (this->windowCamera) {
            ImGui::SetNextWindowPos(ImVec2(980, 26), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(300, 128), ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Camera", &this->windowCamera, ImGuiWindowFlags_NoCollapse)) {
                ImGui::Checkbox("Freelook", &cameraFreelook);
                ImGui::SliderFloat3("position", (float*)&camera->position, -20.0f, 20.0f);
                ImGui::SliderFloat3("front", (float*)&camera->front, -1.0f, 1.0f);
                ImGui::End();
            }
        }

        // Properties windows.
        for (auto& [id, type] : scene->objectMap) {
            if (!this->windowProperties[id]) {
                continue;
            }
            ImGui::SetNextWindowSize(ImVec2(300, 600), ImGuiCond_FirstUseEver);
            char windowTitle[256] = "properties";
            //            sprintf(windowTitle, "%s - properties",
            //            this->scene->objects.at(i)->name.c_str());
            if (ImGui::Begin(windowTitle, &this->windowProperties[id],
                             ImGuiWindowFlags_NoCollapse)) {
                switch (ObjectType(type)) {
                case UNDEFINED:
                    break;
                case SHADER_SOURCE_FILE: {
                    this->propertiesShaderSourceFile(
                        this->scene->getObjectByID<ShaderSourceFile>(id));
                    break;
                }
                case SHADER_PROGRAM: {
                    this->propertiesShaderProgram(this->scene->getObjectByID<ShaderProgram>(id));
                    break;
                }
                }
                ImGui::End();
            }
        }

        // Modal popup for creating a new object.
        if (this->popupNewObject) {
            ImGui::OpenPopup("New object");
            this->popupNewObject = false;
        }
        if (ImGui::BeginPopupModal("New object", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Object type: %s", this->newObjectTypeName.c_str());
            static char name[256];
            ImGui::Text("Object name:");
            ImGui::SameLine();
            ImGui::InputText("", name, IM_ARRAYSIZE(name));
            if (ImGui::Button("Create")) {
                ObjectID newId = this->scene->createObject(this->newObjectType, name);
                this->windowProperties[newId] = false;
                name[0]                       = '\0';
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
}

void UI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::updateWindowTitle() {
    char newTitle[256];
    std::string basename =
        this->appConfig->sceneFile.substr(this->appConfig->sceneFile.find_last_of("/\\") + 1);
    sprintf(newTitle, "%s - Hazard", basename.c_str());
    glfwSetWindowTitle(this->window, newTitle);
}

void UI::propertiesShaderSourceFile(ShaderSourceFile* optr) {
    ImGui::Text("File:");
    ImGui::SameLine();
    ImGui::InputText("", (char*)(optr->filename.c_str()), 512, ImGuiInputTextFlags_ReadOnly);
    ImGui::SameLine();
    if (ImGui::Button("Select")) {
        this->windowOpenShader = true;
    }

    // Open shader file dialog.
    if (this->windowOpenShader) {
        // Instantiate.
        ImGuiFileDialog::Instance()->OpenDialog("OpenShader", "Open shader file",
                                                ".vert .frag .geom{.vert,.frag,.geom},.*", ".");

        // Display configuration.
        // Set default window size.
        ImGui::SetNextWindowSize(ImVec2(700, 450), ImGuiCond_FirstUseEver);
        // Highlight shader files with bright green.
        ImGuiFileDialog::Instance()->SetExtentionInfos(".frag", ImVec4(0.1, 1, 0.2, 1));
        ImGuiFileDialog::Instance()->SetExtentionInfos(".vert", ImVec4(0, 1, 0.5, 1));
        ImGuiFileDialog::Instance()->SetExtentionInfos(".geom", ImVec4(0.2, 1, 0.3, 1));

        // Display.
        if (ImGuiFileDialog::Instance()->Display("OpenShader")) {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                // action
                optr->filename = filePathName;
            }

            // close
            ImGuiFileDialog::Instance()->Close();
            this->windowOpenShader = false;
        }
    }
}
void UI::propertiesShaderProgram(ShaderProgram* optr) {
    std::map<const char*, unsigned int> objs;
    for (auto& [id, type] : this->scene->objectMap) {
        if (type != SHADER_SOURCE_FILE) {
            continue;
        }
        objs.insert(std::pair<const char*, unsigned int>(
            this->scene->getObjectByID<ShaderSourceFile>(id)->name.c_str(), type));
    }

    static const char* current_vertex;
    if (optr->vertexShaderFileID > 0) {
        current_vertex =
            this->scene->getObjectByID<ShaderSourceFile>(optr->vertexShaderFileID)->name.c_str();
    }
    static const char* current_fragment;
    if (optr->fragmentShaderFileID > 0) {
        current_fragment =
            this->scene->getObjectByID<ShaderSourceFile>(optr->fragmentShaderFileID)->name.c_str();
    }

    // Vertex shader selector
    ImGui::Text("Vertex shader:  ");
    ImGui::SameLine();
    ImGui::PushItemWidth(180);
    if (ImGui::BeginCombo("##comboVertex", current_vertex)) {
        for (auto [name, id] : objs) {
            bool is_selected;
            is_selected = current_vertex && (current_vertex == name);
            if (ImGui::Selectable(name, is_selected)) {
                current_vertex           = name;
                optr->vertexShaderFileID = objs.find(name)->second;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();

    // Fragment shader selector
    ImGui::Text("Fragment shader:");
    ImGui::SameLine();
    ImGui::PushItemWidth(180);
    if (ImGui::BeginCombo("##comboFragment", current_fragment)) {
        for (auto [name, id] : objs) {
            bool is_selected;
            is_selected = current_fragment && (current_fragment == name);
            if (ImGui::Selectable(name, is_selected)) {
                current_fragment           = name;
                optr->fragmentShaderFileID = objs.find(name)->second;
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::PopItemWidth();
}
