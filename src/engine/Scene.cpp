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

void Scene::draw() {
    stats->refresh();

    mat4 projection =
        perspective(radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 200.0f);
    mat4 view = camera->getViewMatrix();

    for (auto& o : objects<Cube>()) {
        stats->drawCalls += o->draw(projection, view);
    }
}

void Scene::loadFromFile(const char* pFilename) {
    filename = pFilename;
    load();
}

void Scene::resolveAndInit() {
    for (auto& shader : objects<ShaderProgram>()) {
        shader->resolveLinks(objects<ShaderSourceFile>());
        shader->init();
    }
    for (auto& c : objects<Cube>()) {
        c->resolveLinks(objects<ShaderProgram>());
        c->init();
    }
}

// (De)serialization here is done in semi-manual way instead of macros because seems that
// nlohmann_json has issues with templating pointers (and I need at least camera to be a pointer
// here. Probably one day I'll make it more pretty.

void Scene::load() {
    std::ifstream ifs(filename);
    if (ifs.good()) {
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
                auto o = std::make_shared<ShaderSourceFile>(it.get<ShaderSourceFile>());
                objectsShaderSourceFile.push_back(o);
                break;
            }
            case SHADER_PROGRAM: {
                auto o = std::make_shared<ShaderProgram>(it.get<ShaderProgram>());
                objectsShaderProgram.push_back(o);
                break;
            }
            case CUBE: {
                auto o = std::make_shared<Cube>(it.get<Cube>());
                objectsCube.push_back(o);
                break;
            }
            }
            objectMap.insert(std::pair<unsigned int, ObjectType>(it["id"], t));
        }

        std::cout << "Scene loaded." << std::endl;
    } else {
        std::cerr << "Failed to open scene file, using an empty scene." << std::endl;
    }

    resolveAndInit();
}

void Scene::save() {
    json j;
    j["camera"]  = *camera;
    j["objects"] = json::array();
    for (auto& [id, type] : objectMap) {
        json jo;
        switch (ObjectType(type)) {
        case SHADER_SOURCE_FILE:
            j["objects"].push_back(*getObjectByID<ShaderSourceFile>(id));
            break;
        case SHADER_PROGRAM:
            j["objects"].push_back(*getObjectByID<ShaderProgram>(id));
            break;
        case CUBE:
            j["objects"].push_back(*getObjectByID<Cube>(id));
            break;
        }
    }

    std::ofstream ofs(filename);
    ofs << std::setw(4) << j.dump(2) << std::endl;
    std::cout << "Scene saved." << std::endl;
}

std::map<ObjectType, std::string> Scene::listObjectTypes() {
    std::map<ObjectType, std::string> types;
    types.insert(
        std::pair<ObjectType, std::string>(SHADER_SOURCE_FILE, "\xee\x81\x9f Shader source file"));
    types.insert(std::pair<ObjectType, std::string>(SHADER_PROGRAM, "\xee\x82\xbc Shader program"));
    types.insert(std::pair<ObjectType, std::string>(CUBE, "\xee\x80\xa5 Cube"));
    return types;
}

ObjectID Scene::createObject(int t, const char* name) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(1, INT32_MAX);

    int newId = distribution(generator);
    switch (ObjectType(t)) {
    case UNDEFINED:
        std::cerr << "New object type is not defined." << std::endl;
        return 0;
    case SHADER_SOURCE_FILE: {
        auto no = std::make_shared<ShaderSourceFile>(newId, name);
        objectsShaderSourceFile.push_back(no);
        break;
    }
    case SHADER_PROGRAM: {
        objectsShaderProgram.push_back(std::make_shared<ShaderProgram>(newId, name));
        break;
    }
    case CUBE: {
        objectsCube.push_back(std::make_shared<Cube>(newId, name));
        break;
    }
    default:
        std::cerr << "Unsupported object type: " << t << std::endl;
        return 0;
    }

    objectMap.insert(std::pair<ObjectID, ObjectType>(newId, ObjectType(t)));

    std::cout << "Object (ID = " << newId << ") created." << std::endl;
    return newId;
}

// This method is being called only by UI so it prefixes object name with its icon, that's all.
std::string Scene::getObjectDisplayName(ObjectID oid) {
    for (auto& [id, type] : objectMap) {
        if (id == oid) {
            switch (ObjectType(type)) {
            case SHADER_SOURCE_FILE: {
                auto o = getObjectByID<ShaderSourceFile>(id);
                return std::string(o->icon) + " " + o->name;
            }
            case SHADER_PROGRAM: {
                auto o = getObjectByID<ShaderProgram>(id);
                return std::string(o->icon) + " " + o->name;
            }

            case CUBE: {
                auto o = getObjectByID<Cube>(id);
                return std::string(o->icon) + " " + o->name;
            }
            }
        }
    }
    return "";
}
