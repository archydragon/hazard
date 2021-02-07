#ifndef HAZARD_SHADERPROGRAM_H
#define HAZARD_SHADERPROGRAM_H

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "BaseObject.h"
#include "ShaderSourceFile.h"

class ShaderProgram : public BaseObject {
public:
    ShaderProgram();
    ShaderProgram(unsigned int pId, ObjectType pType, const char* pName)
        : BaseObject(pId, pType, pName){};

    void load();
    void resolveLinks(Objects& objects);
    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    unsigned int programID            = 0;
    unsigned int vertexShaderFileID   = 0;
    unsigned int fragmentShaderFileID = 0;

private:
    ShaderSourceFile* vertexShader   = nullptr;
    ShaderSourceFile* fragmentShader = nullptr;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShaderProgram, id, type, name, vertexShaderFileID,
                                   fragmentShaderFileID)

#endif // HAZARD_SHADERPROGRAM_H
