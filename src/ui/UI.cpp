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
                }

                // close
                ImGuiFileDialog::Instance()->Close();
                this->windowOpenScene = false;
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
    }
}

void UI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
