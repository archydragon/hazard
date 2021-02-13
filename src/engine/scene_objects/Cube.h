#ifndef HAZARD_CUBE_H
#define HAZARD_CUBE_H

#include <nlohmann/json.hpp>

#include "../../glm2json.h"
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

    float scale        = 1.0f;
    glm::vec3 position = glm::vec3(0.0f);
    // Rotation contains numbers between 0 and 360 for each axis.
    glm::vec3 rotation = glm::vec3(0.0f);
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Cube, id, type, name, links, scale, position, rotation)

#endif // HAZARD_CUBE_H
