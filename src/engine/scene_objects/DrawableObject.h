#ifndef HAZARD_DRAWABLEOBJECT_H
#define HAZARD_DRAWABLEOBJECT_H

#include <glm/glm.hpp>

#include "ShaderProgram.h"

class DrawableObject {
public:
    DrawableObject()          = default;
    virtual ~DrawableObject() = default;

    virtual unsigned int draw(glm::mat4 projection, glm::mat4 view) {
        return 0;
    }

protected:
    unsigned int vbo = 0;
    unsigned int vao = 0;

    ShaderProgram* shader = nullptr;
};

#endif // HAZARD_DRAWABLEOBJECT_H
