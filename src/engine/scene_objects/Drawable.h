#ifndef HAZARD_DRAWABLE_H
#define HAZARD_DRAWABLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm2json.h>
#include <nlohmann/json.hpp>

#include "ShaderProgram.h"
#include "TSceneObject.h"
#include "drawables/Cube.h"
#include "drawables/IDrawable.h"
#include "drawables/Plane.h"

class Drawable : public TSceneObject<Drawable> {
public:
    Drawable() = default;
    Drawable(ObjectID id, const char* name);
    static std::map<DrawableType, std::string> listObjectTypes();
    void resolveLinks(const ShaderProgram::Objects& objects);
    void init();
    unsigned int draw(glm::mat4 projection, glm::mat4 view);

    ObjectType type  = DRAWABLE;
    const char* icon = "";
    std::unique_ptr<IDrawable> drawable;
    DrawableType drawableType = UNKNOWN;

    float scale        = 1.0f;
    glm::vec3 position = glm::vec3(0.0f);
    // Rotation contains numbers between 0 and 360 for each axis.
    glm::vec3 rotation = glm::vec3(0.0f);

private:
    ShaderProgram* shader = nullptr;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Drawable, id, type, name, links, drawableType, scale, position,
                                   rotation)

#endif // HAZARD_DRAWABLE_H
