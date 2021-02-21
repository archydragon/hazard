#include <iostream>

#include <glad/glad.h>
// glad should be included before glfw3, keep an empty line to distract clang-format
#include <GLFW/glfw3.h>

#include "Config.h"
#include "engine/Camera.h"
#include "engine/Scene.h"
#include "ui/UI.h"

// Those are declared globally as those objects
Config cfg;
Camera camera;

void GLAPIENTRY glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                  GLsizei length, const GLchar* message, const void* userParam);

int main() {
    // Load config.
    cfg = Config();
    cfg.load();

    // Initialize OpenGL.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window;
    window = glfwCreateWindow(cfg.windowWidth, cfg.windowHeight, "Hazard", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); // Initialize GLEW
    if (cfg.windowMaximized) {
        glfwMaximizeWindow(window);
    }

    // Enable vsync
    glfwSwapInterval(1);

    // GLFW Calbacks.
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int fbwidth, int fbheight) {
        glViewport(0, 0, fbwidth, fbheight);
        cfg.saveWindowSize(fbwidth, fbheight);
    });
    glfwSetWindowMaximizeCallback(window, [](GLFWwindow* window, int maximized) {
        if (maximized) {
            cfg.saveWindowMaximizedState(true);
        } else {
            cfg.saveWindowMaximizedState(false);
        }
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        camera.processMouseControl(window, xoffset, yoffset);
    });

    // glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // OpenGL debug logs.
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glMessageCallback, 0);

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "All OpenGL systems fired up." << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    std::cout << "Using GPU: " << glGetString(GL_VENDOR) << " " << glGetString(GL_RENDERER)
              << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Create scene.
    Scene scene(cfg.sceneFile.c_str(), cfg.windowWidth, cfg.windowHeight, &camera);

    // Instantiate Dear Imgui.
    UI ui(window, &cfg, &scene, &camera);

    // Main rendering loop.
    do {
        scene.stats->refresh();

        scene.prerender();

        camera.processKeyboardControl(window);

        ui.initFrame();

        glViewport(0, 0, cfg.windowWidth, cfg.windowHeight);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.render();

        ui.render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwWindowShouldClose(window) == 0);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void GLAPIENTRY glMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                  GLsizei length, const GLchar* message, const void* userParam) {
    if (type == GL_DEBUG_TYPE_ERROR) {
        std::cerr << "OpenGL error: type: 0x" << std::hex << type << std::dec
                  << " severity: " << std::hex << severity << std::dec << " - " << message
                  << std::endl;
    }
}
