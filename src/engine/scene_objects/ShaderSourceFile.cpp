#include <thread>

#include <glad/glad.h>

#include "ShaderSourceFile.h"

ShaderSourceFile::ShaderSourceFile() = default;

void ShaderSourceFile::init() {
    ISceneObject::init();

    std::string content;
    std::ifstream file;

    // Read shader file content.
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(filename);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        content = stream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Failed to read shader file " << filename << std::endl;
        return;
    }

    const char* shaderCode = content.c_str();

    // Compile shader.
    shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    // Check for errors.
    char log[4096];
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 4096, NULL, log);
        std::cerr << "Failed to compile shader." << std::endl
                  << "Source file: " << filename << std::endl
                  << "Error log:" << std::endl
                  << log << std::endl;
    } else {
        std::cout << "Shader " << filename << " loaded and compiled." << std::endl;
        lastFileUpdate = std::filesystem::last_write_time(filename.c_str());
        std::thread watcher(&ShaderSourceFile::watchFileChanges, this);
        watcher.detach();
    }
}

[[noreturn]] void ShaderSourceFile::watchFileChanges() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        auto update = std::filesystem::last_write_time(filename.c_str());
        if (update > lastFileUpdate) {
            lastFileUpdate = update;
            std::cout << "Detected file update: " << filename << std::endl;
            refreshRequired = true;
        }
    }
}
