#ifndef HAZARD_SHADERSOURCEFILE_H
#define HAZARD_SHADERSOURCEFILE_H

#include <nlohmann/json.hpp>

#include "BaseObject.h"

class ShaderSourceFile : public BaseObject {
public:
    ShaderSourceFile();
    ShaderSourceFile(unsigned int pId, ObjectType pType, const char* pName)
        : BaseObject(pId, pType, pName){};

    std::string filename = "cube.vert";
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShaderSourceFile, id, type, name, filename)

#endif // HAZARD_SHADERSOURCEFILE_H
