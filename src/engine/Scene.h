#ifndef HAZARD_SCENE_H
#define HAZARD_SCENE_H

#include "Camera.h"
#include "RenderStats.h"
#include "scene_objects/Drawable.h"
#include "scene_objects/ISceneObject.h"
#include "scene_objects/ShaderProgram.h"
#include "scene_objects/ShaderSourceFile.h"
#include "scene_objects/Texture.h"

class Scene {
public:
    Scene(const char* filename, int screenWidth, int screenHeight, Camera* camera);

    void draw();
    void save();
    void loadFromFile(const char* pFilename);
    static std::map<ObjectType, std::string> listObjectTypes();
    std::string getObjectDisplayName(ObjectID id);
    ObjectID createObject(int t, const char* n);

    template <class C> typename std::vector<C*> objects() {
        std::vector<C*> vec;
        for (auto [id, o] : objectsStorage) {
            auto obj = dynamic_cast<C*>(o.get());
            if (obj) {
                vec.push_back(obj);
            }
        }
        return vec;
    };

    template <class C> void pushObjectFromJSON(json& j) {
        auto o                = std::make_shared<C>(j.get<C>());
        objectsStorage[o->id] = o;
    }

    template <class C> C* getObjectByID(ObjectID id) {
        return dynamic_cast<C*>(objectsStorage[id].get());
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

    std::map<ObjectID, ObjectType> objectMap;
    RenderStats* stats{};

private:
    const char* filename;
    int screenWidth;
    int screenHeight;
    Camera* camera;
    Objects objectsStorage;

    void load();
    void resolveAndInit();
};

#endif // HAZARD_SCENE_H
