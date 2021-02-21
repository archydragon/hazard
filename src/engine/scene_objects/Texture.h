#ifndef HAZARD_TEXTURE_H
#define HAZARD_TEXTURE_H

#include <nlohmann/json.hpp>

#include "ISceneObject.h"

class Texture : public ISceneObject {
public:
    Texture();
    Texture(ObjectID pId, const char* pName) : ISceneObject(pId, pName){};

    const char* icon() override {
        return "\xee\x81\xb5";
    }
    bool init() override;
    ObjectType type = TEXTURE;

    std::string filename;
    unsigned int textureID = 0;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Texture, id, type, name, filename)

#endif // HAZARD_TEXTURE_H
