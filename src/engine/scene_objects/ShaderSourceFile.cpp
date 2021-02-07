#include "ShaderSourceFile.h"

ShaderSourceFile::ShaderSourceFile() = default;

void ShaderSourceFile::load() {
    std::string content;
    std::ifstream file;

    // Read shader file content.
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(this->filename);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        content = stream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Failed to read shader file " << this->filename << std::endl;
        return;
    }

    const char* shaderCode = content.c_str();

    // Compile shader.
    this->shader = glCreateShader(this->shaderType);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);

    // Check for errors.
    char log[4096];
    int success;
    glGetShaderiv(this->shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(this->shader, 4096, NULL, log);
        std::cerr << "Failed to compile shader." << std::endl
                  << "Source file: " << this->filename << std::endl
                  << "Error log:" << std::endl
                  << log << std::endl;
    } else {
        std::cout << "Shader " << this->filename << " loaded and compiled." << std::endl;
    }
}
