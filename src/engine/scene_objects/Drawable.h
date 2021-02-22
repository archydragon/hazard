#ifndef HAZARD_DRAWABLE_H
#define HAZARD_DRAWABLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm2json.h>
#include <nlohmann/json.hpp>

#include "ISceneObject.h"
#include "LightSource.h"
#include "ShaderProgram.h"
#include "drawables/Cube.h"
#include "drawables/IDrawable.h"
#include "drawables/Plane.h"

class Drawable : public ISceneObject {
public:
    Drawable() = default;
    Drawable(ObjectID id, const char* name);
    void resolveLinks(const Objects& objs) override;
    void init() override;
    unsigned int draw(glm::mat4 projection, glm::mat4 view);

    static std::map<DrawableType, std::string> listObjectTypes();

    ObjectType type = DRAWABLE;
    std::unique_ptr<IDrawable> drawable;
    DrawableType drawableType = UNKNOWN;

    const char* icon() override {
        if (!drawable) {
            return "";
        }
        return drawable->icon();
    };
    [[nodiscard]] glm::mat4 calculateModel() const;

    float scale        = 1.0f;
    glm::vec3 position = glm::vec3(0.0f);
    // Rotation contains numbers between 0 and 360 for each axis.
    glm::vec3 rotation = glm::vec3(0.0f);

private:
    ShaderProgram* shader    = nullptr;
    LightSource* lightSource = nullptr;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Drawable, id, type, name, links, drawableType, scale, position,
                                   rotation)

#endif // HAZARD_DRAWABLE_H
