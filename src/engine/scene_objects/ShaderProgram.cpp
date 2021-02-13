#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() = default;

void ShaderProgram::init() {
    // Don't try to init if shaders are not resolved and compiled.
    if (vertexShader == nullptr || fragmentShader == nullptr) {
        return;
    }

    // Attach shaders and link shader program.
    programID = glCreateProgram();
    glAttachShader(programID, vertexShader->shader);
    glAttachShader(programID, fragmentShader->shader);
    glLinkProgram(programID);

    // Check for errors.
    char log[4096];
    int success;
    glGetShaderiv(programID, GL_COMPILE_STATUS, &success);
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
        if (o->id == links["vertexShaderFileID"]) {
            vertexShader             = o.get();
            vertexShader->shaderType = GL_VERTEX_SHADER;
            vertexShader->init();
        } else if (o->id == links["fragmentShaderFileID"]) {
            fragmentShader             = o.get();
            fragmentShader->shaderType = GL_FRAGMENT_SHADER;
            fragmentShader->init();
        }
    }
}

void ShaderProgram::use() const {
    if (programID == 0) {
        std::cerr << "Attempt to use invalid shader program." << std::endl;
        return;
    }
    glUseProgram(programID);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
