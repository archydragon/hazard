#ifndef HAZARD_PLANE_H
#define HAZARD_PLANE_H

#include <nlohmann/json.hpp>

#include "../../glm2json.h"
#include "BaseObject.h"
#include "DrawableObject.h"

class Plane : public DrawableObject, public BaseObject<Plane> {
public:
    Plane();
    Plane(ObjectID id, const char* name) : BaseObject(id, name) {
        links.insert(std::pair<std::string, ObjectID>("shaderProgramID", 0));
    };

    ObjectType type  = PLANE;
    const char* icon = "\xee\x83\x8b";

    void init();
    void resolveLinks(const ShaderProgram::Objects& objects);
    unsigned int draw(glm::mat4 projection, glm::mat4 view) override;

private:
    unsigned int vbo = 0;
    unsigned int vao = 0;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Plane, id, type, name, links, scale, position, rotation)

#endif // HAZARD_PLANE_H
