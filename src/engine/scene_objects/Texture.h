#ifndef HAZARD_TEXTURE_H
#define HAZARD_TEXTURE_H

#include <nlohmann/json.hpp>

#include "TSceneObject.h"

class Texture : public TSceneObject<Texture> {
public:
    Texture();
    Texture(ObjectID pId, const char* pName) : TSceneObject(pId, pName){};

    void init();
    ObjectType type  = TEXTURE;
    const char* icon = "\xee\x81\xb5";

    std::string filename;
    unsigned int textureID = 0;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Texture, id, type, name, filename)

#endif // HAZARD_TEXTURE_H
