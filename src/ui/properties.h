// Definitions for all propreties windows.

#ifndef HAZARD_PROPERTIES_H
#define HAZARD_PROPERTIES_H

#include "../engine/scene_objects/ShaderProgram.h"
#include "../engine/scene_objects/ShaderSourceFile.h"
#include "fields.h"

// Functions based on this template return true if values have been changed.
template <class C> bool properties(C* obj, Scene* scene);

template <> bool properties<ShaderSourceFile>(ShaderSourceFile* obj, Scene* scene) {
    // Shader source file selector.
    return fileSelector("OpenShader", "Open shader file", ".vert .frag .geom", &obj->id,
                        &obj->filename);
}

template <> bool properties<ShaderProgram>(ShaderProgram* obj, Scene* scene) {
    std::map<ObjectID, std::string> links = scene->getObjectsNames<ShaderSourceFile>();

    // Vertex shader selector.
    bool vs =
        linkedObjectSelector("Vertex shader", &obj->id, &obj->links["vertexShaderFileID"], links);
    // Fragment shader selector.
    bool fs = linkedObjectSelector("Fragment shader", &obj->id, &obj->links["fragmentShaderFileID"],
                                   links);

    return (vs || fs);
}

bool drawableCommon(Scene* scene, ObjectID* targetID, ObjectID* shaderID, float* scale,
                    glm::vec3* position, glm::vec3* rotation) {
    std::map<ObjectID, std::string> links = scene->getObjectsNames<ShaderProgram>();

    bool sp = linkedObjectSelector("Shader", targetID, shaderID, links);

    bool scaled = floatSlider("scale", 0.0f, 50.0f, targetID, scale);

    bool moved   = vec3Slider("move", -20, 20, targetID, position);
    bool rotated = vec3Slider("rotate", 0, 360, targetID, rotation);

    return (scaled || sp || moved || rotated);
}

template <> bool properties<Cube>(Cube* obj, Scene* scene) {
    return drawableCommon(scene, &obj->id, &obj->links["shaderProgramID"], &obj->scale,
                          &obj->position, &obj->rotation);
}

template <> bool properties<Plane>(Plane* obj, Scene* scene) {
    return drawableCommon(scene, &obj->id, &obj->links["shaderProgramID"], &obj->scale,
                          &obj->position, &obj->rotation);
}

// Selection entrypoint.
void properties(ObjectID id, ObjectType type, Scene* scene) {
    switch (type) {
    case UNDEFINED:
        break;
    case SHADER_PROGRAM:
        if (properties<ShaderProgram>(scene->getObjectByID<ShaderProgram>(id), scene)) {
            scene->refreshObject<ShaderProgram>(id);
        }
        break;
    case SHADER_SOURCE_FILE:
        if (properties<ShaderSourceFile>(scene->getObjectByID<ShaderSourceFile>(id), scene)) {
            scene->refreshObject<ShaderSourceFile>(id);
        }
        break;
    case CUBE:
        if (properties<Cube>(scene->getObjectByID<Cube>(id), scene)) {
            scene->refreshObject<Cube>(id);
        }
        break;
    case PLANE:
        if (properties<Plane>(scene->getObjectByID<Plane>(id), scene)) {
            scene->refreshObject<Plane>(id);
        }
        break;
    }
}

#endif // HAZARD_PROPERTIES_H
