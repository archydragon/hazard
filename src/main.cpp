#include <iostream>

#include <glad/glad.h>
// glad should be included before glfw3, keep an empty line to distract clang-format

#include <GLFW/glfw3.h>

#include "config.h"

ConfigWorker cfg;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void window_maximize_callback(GLFWwindow* window, int maximized);

int main() {
    // Load config.
    cfg = ConfigWorker();
    cfg.load();

    // Initialize OpenGL.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    GLFWwindow* window; // (In the accompanying source code, this variable is global for simplicity)
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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetWindowMaximizeCallback(window, window_maximize_callback);

    // glad init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "All OpenGL systems fired up." << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;

    const GLubyte* gpuVendor = glGetString(GL_VENDOR);
    const GLubyte* gpuModel  = glGetString(GL_RENDERER);
    std::cout << "Using GPU: " << gpuVendor << " " << gpuModel << std::endl;

    // Main rendering loop.
    do {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwWindowShouldClose(window) == 0);

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int fbwidth, int fbheight) {
    glViewport(0, 0, fbwidth, fbheight);
    cfg.saveWindowSize(fbwidth, fbheight);
}

void window_maximize_callback(GLFWwindow* window, int maximized) {
    if (maximized) {
        cfg.saveWindowMaximizedState(true);
    } else {
        cfg.saveWindowMaximizedState(false);
    }
}
