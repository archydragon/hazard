#ifndef HAZARD_SHADERSOURCEFILE_H
#define HAZARD_SHADERSOURCEFILE_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
    void compile();
    ObjectType type = SHADER_SOURCE_FILE;

    std::string filename;
    unsigned int shaderType = UNDEFINED;
    unsigned int shader     = 0;

private:
    [[noreturn]] void watchFileChanges();
    std::filesystem::file_time_type lastFileUpdate;
    std::string shaderCode = "";
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShaderSourceFile, id, type, name, filename)

#endif // HAZARD_SHADERSOURCEFILE_H
