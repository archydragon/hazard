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

Scene::Scene(const char* pFilename, int wpWidth, int wpHeight, Camera* cam) {
    camera       = cam;
    screenWidth  = wpWidth;
    screenHeight = wpHeight;

    this->load(pFilename);

    for (auto& shader : this->objects<ShaderProgram>()) {
        shader->resolveLinks(this->objects<ShaderSourceFile>());
    }

    for (auto& shader : this->objects<ShaderSourceFile>()) {
        shader->load();
    }

    for (auto& shader : this->objects<ShaderProgram>()) {
        shader->load();
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // clang-format off
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    // clang-format on
    // first, configure the cube's VAO (and VBO)
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->vao);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Scene::draw() {
    mat4 projection =
        perspective(radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 200.0f);
    mat4 view  = camera->getViewMatrix();
    mat4 model = mat4(0.5f);

    for (auto& shader : this->objects<ShaderProgram>()) {
        shader->use();
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);
        shader->setMat4("model", model);
    }

    // FIXME: it throws one GL_INVALID_OPERATION error for the very first frame.
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "SHADER OpenGL error: 0x" << std::hex << err << std::endl;
    }

    glBindVertexArray(this->vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void Scene::load(const char* pFilename) {
    this->filename = pFilename;
    this->load();
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

        *this->camera = j["camera"].get<Camera>();
        for (auto& it : j["objects"]) {
            const ObjectType t = it["type"];
            switch (t) {
            case UNDEFINED:
                break;
            case SHADER_SOURCE_FILE: {
                this->objectsShaderSourceFile.push_back(
                    std::make_shared<ShaderSourceFile>(it.get<ShaderSourceFile>()));
                break;
            }
            case SHADER_PROGRAM: {
                this->objectsShaderProgram.push_back(
                    std::make_shared<ShaderProgram>(it.get<ShaderProgram>()));
                break;
            }
            }
            this->objectMap.insert(std::pair<unsigned int, ObjectType>(it["id"], t));
        }

        //        std::cout << "Scene has " << this->objects.size() << " objects." << std::endl;
        std::cout << "Scene loaded." << std::endl;
    } else {
        std::cerr << "Failed to open scene file, using an empty scene." << std::endl;
    }
}

void Scene::save() {
    json j;
    j["camera"]  = *this->camera;
    j["objects"] = json::array();
    for (auto& [id, type] : this->objectMap) {
        json jo;
        switch (ObjectType(type)) {
        case SHADER_SOURCE_FILE:
            j["objects"].push_back(*this->getObjectByID<ShaderSourceFile>(id));
            break;
        case SHADER_PROGRAM:
            j["objects"].push_back(*this->getObjectByID<ShaderProgram>(id));
            break;
        }
    }

    std::ofstream ofs(this->filename);
    ofs << std::setw(4) << j.dump(2) << std::endl;
    std::cout << "Scene saved." << std::endl;
}

std::map<ObjectType, std::string> Scene::listObjectTypes() {
    std::map<ObjectType, std::string> types;
    types.insert(
        std::pair<ObjectType, std::string>(SHADER_SOURCE_FILE, "\xee\x81\x9f Shader source file"));
    types.insert(std::pair<ObjectType, std::string>(SHADER_PROGRAM, "\xee\x82\xbc Shader program"));
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
        this->objectsShaderSourceFile.push_back(no);
        break;
    }
    case SHADER_PROGRAM: {
        this->objectsShaderProgram.push_back(std::make_shared<ShaderProgram>(newId, name));
        break;
    }
    default:
        std::cerr << "Unsupported object type: " << t << std::endl;
        return 0;
    }

    this->objectMap.insert(std::pair<ObjectID, ObjectType>(newId, ObjectType(t)));

    std::cout << "Object (ID = " << newId << ") created." << std::endl;
    return newId;
}
