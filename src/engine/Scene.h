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
    void renameObject(ObjectID id, const std::string& newName);
    void deleteObject(ObjectID id);
    void refreshObject(ObjectID id, bool needRebuild = true);
    std::vector<ISceneObject*> getAncestors(ObjectID id);

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

    ISceneObject* getBaseObjectByID(ObjectID id) {
        return objectsStorage[id].get();
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

    Objects objectsStorage;
    std::vector<ObjectID> sortedObjectIDs;
    RenderStats* stats{};

private:
    const char* filename;
    int screenWidth;
    int screenHeight;
    Camera* camera;
    std::map<ObjectID, std::vector<ISceneObject*>> ancestors;

    void load();
    void resolveAndInit();
    void rebuildAncestors();
    void sortIDs();
};

#endif // HAZARD_SCENE_H
