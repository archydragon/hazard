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
#include "scene_objects/BaseObject.h"
#include "scene_objects/ShaderSourceFile.h"

using nlohmann::json;

Scene::Scene(const char* pFilename, int wpWidth, int wpHeight, Camera* cam) {
    camera       = cam;
    screenWidth  = wpWidth;
    screenHeight = wpHeight;

    filename = pFilename;
    this->load();

    this->shader = new Shader("assets/shaders/cube.vert", "assets/shaders/cube.frag");

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
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(vao);
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
    shader->use();
    unsigned int err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "SHADER OpenGL error: 0x" << std::hex << err << std::endl;
    }
    mat4 projection =
        perspective(radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 200.0f);
    mat4 view  = camera->getViewMatrix();
    mat4 model = mat4(0.5f);
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);

    glBindVertexArray(vao);
    err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cout << "BIND OpenGL error: 0x" << std::hex << err << std::endl;
    }
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
        //        this->objects = j["objects"].get<std::vector<BaseObject>>();
        for (auto& it : j["objects"]) {
            switch (ObjectType(it["type"])) {
            case UNDEFINED:
                break;
            case SHADER_SOURCE_FILE:
                objects.push_back(std::make_unique<ShaderSourceFile>(it.get<ShaderSourceFile>()));
                break;
            case SHADER_PROGRAM:
                break;
            }
        }

        std::cout << "Scene has " << this->objects.size() << " objects." << std::endl;
        std::cout << "Scene loaded." << std::endl;
    } else {
        std::cerr << "Failed to open scene file, using default settings." << std::endl;
    }
}

void Scene::save() {
    json j;
    j["camera"]  = *this->camera;
    j["objects"] = json::array();
    for (auto& o : this->objects) {
        json jo;
        switch (o->type) {
        case UNDEFINED:
            break;
        case SHADER_SOURCE_FILE:
            jo = (*dynamic_cast<ShaderSourceFile*>(o.get()));
            break;
        case SHADER_PROGRAM:
            break;
        }
        j["objects"].push_back(jo);
    }

    std::ofstream ofs(this->filename);
    ofs << std::setw(4) << j.dump(2) << std::endl;
    std::cout << "Scene saved." << std::endl;
}

std::map<ObjectType, std::string> Scene::listObjectTypes() {
    std::map<ObjectType, std::string> types;
    types.insert(std::pair<ObjectType, std::string>(SHADER_SOURCE_FILE, "Shader source file"));
    types.insert(std::pair<ObjectType, std::string>(SHADER_PROGRAM, "Shader program"));
    return types;
}

void Scene::createObject(int t, const char* n) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(1, INT32_MAX);

    int newId = distribution(generator);
    switch (ObjectType(t)) {
    case SHADER_SOURCE_FILE:
        objects.push_back(std::make_unique<ShaderSourceFile>(newId, ObjectType(t), n));
        break;
    case UNDEFINED:
        std::cerr << "New object type is not defined." << std::endl;
        return;
    case SHADER_PROGRAM:
        std::cout << "Not implemented yet." << std::endl;
        return;
    default:
        std::cerr << "Unsupported object type: " << t << std::endl;
        return;
    }

    std::cout << "Object (ID = " << newId << ") created." << std::endl;
}
