#ifndef HAZARD_SHADERSOURCEFILE_H
#define HAZARD_SHADERSOURCEFILE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <nlohmann/json.hpp>

#include "BaseObject.h"

class ShaderSourceFile : public BaseObject {
public:
    ShaderSourceFile();
    ShaderSourceFile(unsigned int pId, ObjectType pType, const char* pName)
        : BaseObject(pId, pType, pName){};

    void load();

    std::string filename;
    unsigned int shaderType;
    unsigned int shader;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShaderSourceFile, id, type, name, filename)

#endif // HAZARD_SHADERSOURCEFILE_H
