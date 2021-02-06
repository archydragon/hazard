#ifndef HAZARD_SCENE_H
#define HAZARD_SCENE_H

#include "./scene_objects/BaseObject.h"
#include "Camera.h"
#include "Shader.h"

class Scene {
public:
    Scene(const char* pFilename, int wpWidth, int wpHeight, Camera* cam);
    void draw();
    void save();
    void load(const char* pFilename);
    static std::map<ObjectType, std::string> listObjectTypes();
    void createObject(int t, const char* n);

    std::vector<std::unique_ptr<BaseObject>> objects;

    const char* getObjectNameByID(unsigned int id);

private:
    const char* filename;
    int screenWidth;
    int screenHeight;
    Camera* camera;
    Shader* shader;
    unsigned int vao = 0;
    unsigned int vbo = 0;

    void load();
};

#endif // HAZARD_SCENE_H
