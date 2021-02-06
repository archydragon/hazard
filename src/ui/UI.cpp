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
    io.Fonts->AddFontFromFileTTF("editor_assets/RobotoMono-Regular.ttf", 18.0f);
    io.Fonts->Build();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(NULL);

    appConfig = config;
    camera    = pCamera;
    scene     = pScene;

    this->updateWindowTitle();
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
                for (int i = 0; i < scene->objects.size(); i++) {
                    if (ImGui::Selectable(scene->objects[i]->name.c_str(), false,
                                          ImGuiSelectableFlags_AllowDoubleClick)) {
                        if (ImGui::IsMouseDoubleClicked(0)) {
                            std::cout << "double cliek " << i << std::endl;
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
                this->scene->createObject(this->newObjectType, name);
                name[0] = '\0';
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
