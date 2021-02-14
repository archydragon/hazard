#ifndef HAZARD_SCENE_H
#define HAZARD_SCENE_H

#include "Camera.h"
#include "RenderStats.h"
#include "scene_objects/Drawable.h"
#include "scene_objects/ShaderProgram.h"
#include "scene_objects/ShaderSourceFile.h"
#include "scene_objects/TSceneObject.h"
#include "scene_objects/Texture.h"

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
    template <> typename Drawable::Objects objects<Drawable>() {
        return objectsDrawable;
    }
    template <> typename Texture::Objects objects<Texture>() {
        return objectsTexture;
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

    ShaderProgram::Objects objectsShaderProgram;
    ShaderSourceFile::Objects objectsShaderSourceFile;
    Drawable::Objects objectsDrawable;
    Texture::Objects objectsTexture;

    void load();
    void resolveAndInit();
};

#endif // HAZARD_SCENE_H
