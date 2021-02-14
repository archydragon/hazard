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
    // Shader selection.
    std::map<ObjectID, std::string> links = scene->getObjectsNames<ShaderSourceFile>();
    // Vertex shader selector.
    bool vs =
        linkedObjectSelector("Vertex shader", &obj->id, &obj->links["vertexShaderFileID"], links);
    // Fragment shader selector.
    bool fs = linkedObjectSelector("Fragment shader", &obj->id, &obj->links["fragmentShaderFileID"],
                                   links);

    // Texture selection.
    links   = scene->getObjectsNames<Texture>();
    bool ts = linkedObjectSelector("Texture", &obj->id, &obj->links["textureID"], links);

    return (vs || fs || ts);
}

template <> bool properties<Drawable>(Drawable* obj, Scene* scene) {
    std::map<ObjectID, std::string> links = scene->getObjectsNames<ShaderProgram>();

    bool sp = linkedObjectSelector("Shader", &obj->id, &obj->links["shaderProgramID"], links);

    bool scaled = floatSlider("scale", 0.0f, 50.0f, &obj->id, &obj->scale);

    bool moved   = vec3Slider("move", -20, 20, &obj->id, &obj->position);
    bool rotated = vec3Slider("rotate", 0, 360, &obj->id, &obj->rotation);

    return (scaled || sp || moved || rotated);
}

template <> bool properties<Texture>(Texture* obj, Scene* scene) {
    // Shader source file selector.
    return fileSelector("OpenTexture", "Open texture image", ".png .jpeg .jpg .bmp .tga", &obj->id,
                        &obj->filename);
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
    case DRAWABLE:
        if (properties<Drawable>(scene->getObjectByID<Drawable>(id), scene)) {
            scene->refreshObject<Drawable>(id);
        }
        break;
    case TEXTURE:
        if (properties<Texture>(scene->getObjectByID<Texture>(id), scene)) {
            scene->refreshObject<Texture>(id);
        }
        break;
    }
}

#endif // HAZARD_PROPERTIES_H
