// Definitions for all propreties windows.

#ifndef HAZARD_PROPERTIES_H
#define HAZARD_PROPERTIES_H

#include "../engine/scene_objects/ShaderProgram.h"
#include "../engine/scene_objects/ShaderSourceFile.h"
#include "fields.h"

template <class C> void properties(C* obj, Scene* scene);

template <> void properties<ShaderSourceFile>(ShaderSourceFile* obj, Scene* scene) {
    // Shader source file selector.
    fileSelector("OpenShader", "Open shader file", ".vert .frag .geom", &obj->id, &obj->filename);
}

template <> void properties<ShaderProgram>(ShaderProgram* obj, Scene* scene) {
    std::map<ObjectID, std::string> links = scene->getObjectsNames<ShaderSourceFile>();

    // Vertex shader selector.
    linkedObjectSelector("Vertex shader", &obj->id, &obj->vertexShaderFileID, links);
    // Fragment shader selector.
    linkedObjectSelector("Fragment shader", &obj->id, &obj->fragmentShaderFileID, links);
}

// Selection entrypoint.
void properties(ObjectID id, ObjectType type, Scene* scene) {
    switch (type) {
    case UNDEFINED:
        break;
    case SHADER_PROGRAM:
        properties<ShaderProgram>(scene->getObjectByID<ShaderProgram>(id), scene);
        break;
    case SHADER_SOURCE_FILE:
        properties<ShaderSourceFile>(scene->getObjectByID<ShaderSourceFile>(id), scene);
        break;
    }
}

#endif // HAZARD_PROPERTIES_H
