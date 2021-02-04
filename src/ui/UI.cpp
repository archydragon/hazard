#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "UI.h"

bool UI::cameraFreelook = false;

UI::UI(GLFWwindow* window, Scene* pScene, Camera* pCamera) {
    // IMGUI initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    io.Fonts->AddFontFromFileTTF("editor_assets/RobotoMono-Regular.ttf", 18.0f);
    io.Fonts->Build();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(NULL);

    camera = pCamera;
    scene  = pScene;
}

UI::~UI() {
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::initFrame() {
    bool save = false;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    {
        // Main menu.
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Scene", true)) {
                ImGui::MenuItem("New");
                ImGui::MenuItem("Load");
                save = ImGui::MenuItem("Save");

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // Camera window.
        ImGui::SetNextWindowPos(ImVec2(980, 26), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, 128), ImGuiCond_FirstUseEver);
        if (ImGui::Begin("Camera", NULL, ImGuiWindowFlags_NoCollapse)) {
            ImGui::Checkbox("Freelook", &cameraFreelook);
            ImGui::SliderFloat3("position", (float*)&camera->position, -20.0f, 20.0f);
            ImGui::SliderFloat3("front", (float*)&camera->front, -1.0f, 1.0f);
            ImGui::End();
        }

        if (save) {
            scene->save();
        }
    }
}

void UI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
