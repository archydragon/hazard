#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() = default;

void ShaderProgram::load() {
    // Attach shaders and link shader program.
    this->programID = glCreateProgram();
    glAttachShader(this->programID, this->vertexShader->shader);
    glAttachShader(this->programID, this->fragmentShader->shader);
    glLinkProgram(this->programID);

    // Check for errors.
    char log[4096];
    int success;
    glGetShaderiv(this->programID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(this->programID, 4096, NULL, log);
        std::cerr << "Failed to link shader program." << std::endl
                  << "Error log:" << std::endl
                  << log << std::endl;
    } else {
        std::cout << "Shader linked successfully." << std::endl;
    }
}

void ShaderProgram::resolveLinks(const ShaderSourceFile::Objects& objects) {
    for (auto& o : objects) {
        if (o->id == this->vertexShaderFileID) {
            this->vertexShader             = o.get();
            this->vertexShader->shaderType = GL_VERTEX_SHADER;
        } else if (o->id == this->fragmentShaderFileID) {
            this->fragmentShader             = o.get();
            this->fragmentShader->shaderType = GL_FRAGMENT_SHADER;
        }
    }
}

void ShaderProgram::use() const {
    glUseProgram(this->programID);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->programID, name.c_str()), 1, GL_FALSE,
                       &mat[0][0]);
}
