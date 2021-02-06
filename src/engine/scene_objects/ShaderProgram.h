#ifndef HAZARD_SHADERPROGRAM_H
#define HAZARD_SHADERPROGRAM_H

#include <nlohmann/json.hpp>

#include "BaseObject.h"

class ShaderProgram : public BaseObject {
public:
    ShaderProgram();
    ShaderProgram(unsigned int pId, ObjectType pType, const char* pName)
        : BaseObject(pId, pType, pName){};

    unsigned int vertexShaderFileID   = 0;
    unsigned int fragmentShaderFileID = 0;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShaderProgram, id, type, name, vertexShaderFileID,
                                   fragmentShaderFileID)

#endif // HAZARD_SHADERPROGRAM_H
