#ifndef HAZARD_DRAWABLEOBJECT_H
#define HAZARD_DRAWABLEOBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ShaderProgram.h"

class DrawableObject {
public:
    DrawableObject()          = default;
    virtual ~DrawableObject() = default;

    float scale        = 1.0f;
    glm::vec3 position = glm::vec3(0.0f);
    // Rotation contains numbers between 0 and 360 for each axis.
    glm::vec3 rotation = glm::vec3(0.0f);

    virtual unsigned int draw(glm::mat4 projection, glm::mat4 view) {
        // Init.
        glm::mat4 model = glm::mat4(1.0f);

        // Move.
        model = glm::translate(model, position);

        // Rotate.
        glm::mat4 rotationMatrix =
            glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
            glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
            glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        glm::mat4 translate    = glm::translate(glm::mat4(1.0), position);
        glm::mat4 invTranslate = glm::inverse(translate);

        glm::mat4 transform = translate * rotationMatrix * invTranslate;
        model               = transform * model;

        // Scale.
        model = glm::scale(model, glm::vec3(scale));

        if (!shader) {
            return 0;
        }

        shader->use();
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        shader->setMat4("model", model);

        return 0;
    };

protected:
    ShaderProgram* shader = nullptr;
};

#endif // HAZARD_DRAWABLEOBJECT_H
