#include <iomanip>
#include <iostream>
#include <map>
#include <random>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>

#include "Camera.h"
#include "Scene.h"

using nlohmann::json;

Scene::Scene(const char* filename, int screenWidth, int screenHeight, Camera* camera)
    : filename(filename), screenWidth(screenWidth), screenHeight(screenHeight), camera(camera) {
    load();
    stats = new RenderStats;
}

std::map<ObjectType, std::string> Scene::listObjectTypes() {
    std::map<ObjectType, std::string> types;
    types.insert(
        std::pair<ObjectType, std::string>(SHADER_SOURCE_FILE, "\xee\x81\x9f Shader source file"));
    types.insert(std::pair<ObjectType, std::string>(SHADER_PROGRAM, "\xee\x82\xbc Shader program"));
    types.insert(std::pair<ObjectType, std::string>(TEXTURE, "\xee\x81\xb5 Texture"));
    return types;
}

void Scene::draw() {
    stats->refresh();

    mat4 projection =
        perspective(radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 200.0f);
    mat4 view = camera->getViewMatrix();

    for (auto& o : objects<Drawable>()) {
        stats->drawCalls += o->draw(projection, view);
    }
}

void Scene::resolveAndInit() {
    for (auto [id, o] : objectsStorage) {
        if (!o->links.empty()) {
            o->resolveLinks(objectsStorage);
        }
    }

    for (auto [id, o] : objectsStorage) {
        o->init();
    }

    sortIDs();
    rebuildAncestors();
}

void Scene::rebuildAncestors() {
    ancestors.clear();
    for (auto [id, o] : objectsStorage) {
        if (o->links.empty()) {
            continue;
        }

        for (auto lo : o->pLinks) {
            ancestors[lo->id].push_back(o.get());
        }
    }
}

void Scene::loadFromFile(const char* pFilename) {
    filename = pFilename;
    load();
}

// (De)serialization here is done in semi-manual way instead of macros because seems that
// nlohmann_json has issues with templating pointers (and I need at least camera to be a pointer
// here. Probably one day I'll make it more pretty.

void Scene::load() {
    std::ifstream ifs(filename);
    if (ifs.good()) {
        sortedObjectIDs.clear();
        std::cout << "Using " << filename << " to load scene." << std::endl;
        json j;
        ifs >> j;

        *camera = j["camera"].get<Camera>();
        for (auto& it : j["objects"]) {
            const ObjectType t = it["type"];
            switch (t) {
            case UNDEFINED:
                break;
            case SHADER_SOURCE_FILE: {
                pushObjectFromJSON<ShaderSourceFile>(it);
                break;
            }
            case SHADER_PROGRAM: {
                pushObjectFromJSON<ShaderProgram>(it);
                break;
            }
            case DRAWABLE: {
                pushObjectFromJSON<Drawable>(it);
                break;
            }
            case TEXTURE: {
                pushObjectFromJSON<Texture>(it);
                break;
            }
            }
            sortedObjectIDs.push_back(it["id"]);
        }
        std::cout << "Scene loaded." << std::endl;
    } else {
        std::cerr << "Failed to open scene file, using an empty scene." << std::endl;
    }

    resolveAndInit();
    sortIDs();
}

void Scene::save() {
    json j;
    j["camera"]  = *camera;
    j["objects"] = json::array();
    for (auto& [id, o] : objectsStorage) {
        // FIXME: it looks like a dirty hack, prettify.
        if (dynamic_cast<ShaderProgram*>(o.get())) {
            j["objects"].push_back(*dynamic_cast<ShaderProgram*>(o.get()));
        }
        if (dynamic_cast<ShaderSourceFile*>(o.get())) {
            j["objects"].push_back(*dynamic_cast<ShaderSourceFile*>(o.get()));
        }
        if (dynamic_cast<Drawable*>(o.get())) {
            j["objects"].push_back(*dynamic_cast<Drawable*>(o.get()));
        }
        if (dynamic_cast<Texture*>(o.get())) {
            j["objects"].push_back(*dynamic_cast<Texture*>(o.get()));
        }
    }

    std::ofstream ofs(filename);
    ofs << std::setw(4) << j.dump(2) << std::endl;
    std::cout << "Scene saved." << std::endl;
}

ObjectID Scene::createObject(int t, const char* name) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(1, INT32_MAX);
    int newID = distribution(generator);

    if (t > DRAWABLE) {
        auto o                = std::make_shared<Drawable>(newID, name);
        o->drawableType       = DrawableType(t);
        objectsStorage[newID] = o;
        // Set type back to DRAWABLE for switch-case below.
        t = DRAWABLE;
    }

    switch (ObjectType(t)) {
    case UNDEFINED:
        std::cerr << "New object type is not defined." << std::endl;
        return 0;
    case SHADER_SOURCE_FILE: {
        objectsStorage[newID] = std::make_shared<ShaderSourceFile>(newID, name);
        break;
    }
    case SHADER_PROGRAM: {
        objectsStorage[newID] = std::make_shared<ShaderProgram>(newID, name);
        break;
    }
    case DRAWABLE: {
        break;
    }
    case TEXTURE: {
        objectsStorage[newID] = (std::make_shared<Texture>(newID, name));
        break;
    }
    default:
        std::cerr << "Unsupported object type: " << t << std::endl;
        return 0;
    }
    sortedObjectIDs.push_back(newID);
    sortIDs();

    std::cout << "Object (ID = " << newID << ") created." << std::endl;
    return newID;
}

void Scene::renameObject(ObjectID id, const std::string& newName) {
    objectsStorage[id]->name = newName;
    sortIDs();
}

void Scene::deleteObject(ObjectID id) {
    sortedObjectIDs.erase(std::remove(sortedObjectIDs.begin(), sortedObjectIDs.end(), id),
                          sortedObjectIDs.end());
    objectsStorage.erase(id);
    ancestors.erase(id);
    std::cout << "Object (ID = " << id << ") deleted." << std::endl;

    // Remove this ID mentions in all links and re-resolve the whole scene.
    for (auto [_, obj] : objectsStorage) {
        for (auto [linkedKey, linkedID] : obj->links) {
            if (id == linkedID) {
                obj->links[linkedKey] = 0;
            }
        }
    }
    resolveAndInit();
}

// This method is being called only by UI so it prefixes object name with its icon, that's all.
std::string Scene::getObjectDisplayName(ObjectID oid) {
    for (auto [id, o] : objectsStorage) {
        if (o && id == oid) {
            return std::string(o->icon()) + " " + o->name;
        }
    }
    return "";
}

// Two-way object refresher.
void Scene::refreshObject(ObjectID id, bool needRebuild) {
    // First, re-resolve links to children.
    auto obj = objectsStorage[id].get();
    obj->resolveLinks(objectsStorage);

    // If it was the first call from this function from UI update, we also need to rebuild
    // ancestors cache.
    if (needRebuild) {
        rebuildAncestors();
    }

    // Re-initialize object and its ancestors.
    obj->init();
    for (auto a : ancestors[id]) {
        a->init();
    }
}

// This helper function is called each time when we load scene or create a new object.
// Otherwise, it would need to sort objects each frame as it is being called by Imgui.
void Scene::sortIDs() {
    std::sort(sortedObjectIDs.begin(), sortedObjectIDs.end(), [this](ObjectID a, ObjectID b) {
        return objectsStorage[a]->name < objectsStorage[b]->name;
    });
}

std::vector<ISceneObject*> Scene::getAncestors(ObjectID id) {
    std::vector<ISceneObject*> objectAncestors;
    for (auto obj : ancestors[id]) {
        objectAncestors.push_back(obj);
    }
    return objectAncestors;
}
