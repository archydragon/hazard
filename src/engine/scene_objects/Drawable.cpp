#include "Drawable.h"

Drawable::Drawable(ObjectID id, const char* name) : ISceneObject(id, name) {
    registerLinkName("shaderProgramID");
    registerLinkName("lightSourceID");
}

std::map<DrawableType, std::string> Drawable::listObjectTypes() {
    std::map<DrawableType, std::string> types;
    types.insert(std::pair<DrawableType, std::string>(CUBE, "\xee\x80\xa5 Cube"));
    types.insert(std::pair<DrawableType, std::string>(PLANE, "\xee\x83\x8b Plane"));
    return types;
}

void Drawable::resolveLinks(const Objects& objs) {
    ISceneObject::resolveLinks(objs);

    if (links["shaderProgramID"] > 0) {
        shader = (ShaderProgram*)objs.at(links["shaderProgramID"]).get();
    } else {
        shader = nullptr;
    }
    if (links["lightSourceID"] > 0) {
        lightSource = (LightSource*)objs.at(links["lightSourceID"]).get();
    } else {
        lightSource = nullptr;
    }
}

void Drawable::init() {
    switch (drawableType) {
    case CUBE:
        drawable = std::make_unique<Cube>();
        break;
    case PLANE:
        drawable = std::make_unique<Plane>();
        break;
    }
    drawable->init();
}

unsigned int Drawable::draw(glm::mat4 projection, glm::mat4 view) {
    glm::mat4 model = calculateModel();

    if (!shader) {
        return 0;
    }

    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);

    if (lightSource) {
        shader->setMat4("lightSpaceMatrix", lightSource->lightSpace);
        shader->setVec3("lightPos", lightSource->position);
        shader->setVec3("lightDir", lightSource->direction);

        glActiveTexture(GL_TEXTURE0 + lightSource->depthMap);
        shader->setInt("depthmap", lightSource->depthMap);
        glBindTexture(GL_TEXTURE_2D, lightSource->depthMap);
    }

    return drawable->draw(projection, view);
}

glm::mat4 Drawable::calculateModel() const {
    glm::mat4 model = glm::mat4(1.0f);

    // Move.
    model = glm::translate(model, position);

    // Rotate.
    glm::mat4 rotationMatrix = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0)) *
                               glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0)) *
                               glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
    glm::mat4 translate    = glm::translate(glm::mat4(1.0), position);
    glm::mat4 invTranslate = glm::inverse(translate);

    glm::mat4 transform = translate * rotationMatrix * invTranslate;
    model               = transform * model;

    // Scale.
    model = glm::scale(model, glm::vec3(scale));
    return model;
}
