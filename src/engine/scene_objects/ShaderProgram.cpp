#include <glad/glad.h>

#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() = default;

ShaderProgram::ShaderProgram(ObjectID id, const char* name) : ISceneObject(id, name) {
    registerLinkName("vertexShaderFileID");
    registerLinkName("fragmentShaderFileID");
    registerLinkName("textureID");
}

void ShaderProgram::resolveLinks(const Objects& objs) {
    ISceneObject::resolveLinks(objs);

    if (links["vertexShaderFileID"] > 0) {
        vertexShader             = (ShaderSourceFile*)objs.at(links["vertexShaderFileID"]).get();
        vertexShader->shaderType = GL_VERTEX_SHADER;
    } else {
        vertexShader = nullptr;
    }
    if (links["fragmentShaderFileID"] > 0) {
        fragmentShader = (ShaderSourceFile*)objs.at(links["fragmentShaderFileID"]).get();
        fragmentShader->shaderType = GL_FRAGMENT_SHADER;
    } else {
        fragmentShader = nullptr;
    }
    if (links["textureID"] > 0) {
        textureDiffuse = (Texture*)objs.at(links["textureID"]).get();
    } else {
        textureDiffuse = nullptr;
    }
}

bool ShaderProgram::init() {
    if (ISceneObject::init()) {
        return true;
    }

    // Don't try to init if shaders are not resolved and compiled.
    if (vertexShader == nullptr || fragmentShader == nullptr) {
        return false;
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
        retrieveUniforms();
    }

    return true;
}

void ShaderProgram::use() const {
    if (programID == 0) {
        std::cerr << "Attempt to use invalid shader program." << std::endl;
        return;
    }
    glUseProgram(programID);
    // TODO: cleanup
    if (textureDiffuse) {
        glActiveTexture(GL_TEXTURE0 + textureDiffuse->textureID);
        setInt("texture_diffuse", textureDiffuse->textureID);
        glBindTexture(GL_TEXTURE_2D, textureDiffuse->textureID);
    }
}

void ShaderProgram::retrieveUniforms() {
    uniforms.clear();
    int numUniforms;
    glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &numUniforms);
    for (int ui = 0; ui < numUniforms; ui++) {
        char name[256];
        unsigned int uType;
        int nLength;
        int uSize;
        glGetActiveUniform(programID, ui, 256, &nLength, &uSize, &uType, name);

        std::string sName = std::string(name);
        // Those uniforms are not supposed to be universallu configurable.
        if (sName == "model" || sName == "view" || sName == "projection") {
            continue;
        }

        uniforms[sName] = ShaderUniform{sName, uType};
    }
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}
