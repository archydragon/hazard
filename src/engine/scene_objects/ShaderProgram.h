#ifndef HAZARD_SHADERPROGRAM_H
#define HAZARD_SHADERPROGRAM_H

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

#include "ISceneObject.h"
#include "ShaderSourceFile.h"
#include "Texture.h"

class ShaderProgram : public ISceneObject {
public:
    ShaderProgram();
    ShaderProgram(ObjectID id, const char* name);

    const char* icon() override {
        return "\xee\x82\xbc";
    }
    void resolveLinks(const Objects& objs) override;
    void init() override;
    void use() const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    void setInt(const std::string& name, int value) const;

    ObjectType type = SHADER_PROGRAM;

    unsigned int programID = 0;

private:
    ShaderSourceFile* vertexShader   = nullptr;
    ShaderSourceFile* fragmentShader = nullptr;
    Texture* textureDiffuse          = nullptr;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShaderProgram, id, type, name, links)

#endif // HAZARD_SHADERPROGRAM_H
