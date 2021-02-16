#ifndef HAZARD_SHADERSOURCEFILE_H
#define HAZARD_SHADERSOURCEFILE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <nlohmann/json.hpp>

#include "ISceneObject.h"

class ShaderSourceFile : public ISceneObject {
public:
    ShaderSourceFile();
    ShaderSourceFile(ObjectID pId, const char* pName) : ISceneObject(pId, pName){};

    const char* icon() override {
        return "\xee\x81\x9f";
    }
    void init() override;
    ObjectType type = SHADER_SOURCE_FILE;

    std::string filename;
    unsigned int shaderType = UNDEFINED;
    unsigned int shader     = 0;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShaderSourceFile, id, type, name, filename)

#endif // HAZARD_SHADERSOURCEFILE_H
