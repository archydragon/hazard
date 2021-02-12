#ifndef HAZARD_CUBE_H
#define HAZARD_CUBE_H

#include <nlohmann/json.hpp>

#include "BaseObject.h"
#include "DrawableObject.h"

class Cube : DrawableObject, public BaseObject<Cube> {
public:
    Cube();
    Cube(ObjectID id, const char* name) : BaseObject(id, name) {
        links.insert(std::pair<std::string, ObjectID>("shaderProgramID", 0));
    };

    ObjectType type  = CUBE;
    const char* icon = "\xee\x80\xa5";

    void init();
    void resolveLinks(const ShaderProgram::Objects& objects);
    unsigned int draw(glm::mat4 projection, glm::mat4 view) override;

    float scale = 1.0f;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Cube, id, type, name, links, scale)

#endif // HAZARD_CUBE_H
