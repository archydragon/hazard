#ifndef HAZARD_SHADERPROGRAM_H
#define HAZARD_SHADERPROGRAM_H

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "BaseObject.h"
#include "ShaderSourceFile.h"

class ShaderProgram : public BaseObject<ShaderProgram> {
public:
    ShaderProgram();
    ShaderProgram(ObjectID pId, const char* pName) : BaseObject(pId, pName) {
        links.insert(std::pair<std::string, ObjectID>("vertexShaderFileID", 0));
        links.insert(std::pair<std::string, ObjectID>("fragmentShaderFileID", 0));
    };

    void init();
    void resolveLinks(const ShaderSourceFile::Objects& objects);
    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    ObjectType type  = SHADER_PROGRAM;
    const char* icon = "\xee\x82\xbc";

    unsigned int programID = 0;

private:
    ShaderSourceFile* vertexShader   = nullptr;
    ShaderSourceFile* fragmentShader = nullptr;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShaderProgram, id, type, name, links)

#endif // HAZARD_SHADERPROGRAM_H
