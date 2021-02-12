#ifndef HAZARD_SHADERSOURCEFILE_H
#define HAZARD_SHADERSOURCEFILE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <nlohmann/json.hpp>

#include "BaseObject.h"

class ShaderSourceFile : public BaseObject<ShaderSourceFile> {
public:
    ShaderSourceFile();
    ShaderSourceFile(ObjectID pId, const char* pName) : BaseObject(pId, pName){};

    void init();
    ObjectType type  = SHADER_SOURCE_FILE;
    const char* icon = "\xee\x81\x9f";

    std::string filename;
    unsigned int shaderType = UNDEFINED;
    unsigned int shader     = 0;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShaderSourceFile, id, type, name, filename)

#endif // HAZARD_SHADERSOURCEFILE_H
