#ifndef HAZARD_SCENE_H
#define HAZARD_SCENE_H

#include "Camera.h"
#include "RenderStats.h"
#include "scene_objects/BaseObject.h"
#include "scene_objects/Cube.h"
#include "scene_objects/Plane.h"
#include "scene_objects/ShaderProgram.h"
#include "scene_objects/ShaderSourceFile.h"

class Scene {
public:
    Scene(const char* filename, int screenWidth, int screenHeight, Camera* camera);

    void draw();
    void save();
    void loadFromFile(const char* pFilename);
    std::map<ObjectID, ObjectType> objectMap;
    static std::map<ObjectType, std::string> listObjectTypes();
    std::string getObjectDisplayName(ObjectID id);
    ObjectID createObject(int t, const char* n);

    template <class C> typename C::Objects objects();
    template <> typename ShaderSourceFile::Objects objects<ShaderSourceFile>() {
        return objectsShaderSourceFile;
    }
    template <> typename ShaderProgram::Objects objects<ShaderProgram>() {
        return objectsShaderProgram;
    }
    template <> typename Cube::Objects objects<Cube>() {
        return objectsCube;
    }
    template <> typename Plane::Objects objects<Plane>() {
        return objectsPlane;
    }

    template <class C> C* getObjectByID(ObjectID id) {
        for (auto o : this->template objects<C>()) {
            if (o->id == id) {
                return o.get();
            }
        }
        return nullptr;
    }

    template <class C> std::map<ObjectID, std::string> getObjectsNames() {
        std::map<ObjectID, std::string> result;
        for (auto o : objects<C>()) {
            result.insert(std::pair<ObjectID, std::string>(o->id, o->name));
        }
        return result;
    }

    template <class C> void refreshObject(ObjectID id) {
        getObjectByID<C>(id)->init();
    }

    RenderStats* stats{};

private:
    const char* filename;
    int screenWidth;
    int screenHeight;
    Camera* camera;
    unsigned int vao = 0;
    unsigned int vbo = 0;

    ShaderProgram::Objects objectsShaderProgram;
    ShaderSourceFile::Objects objectsShaderSourceFile;
    Cube::Objects objectsCube;
    Plane::Objects objectsPlane;

    void load();
    void resolveAndInit();
};

#endif // HAZARD_SCENE_H
