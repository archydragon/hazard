#ifndef HAZARD_SCENE_H
#define HAZARD_SCENE_H

#include "Camera.h"
#include "Shader.h"

class Scene {
public:
    Scene(const char* pFilename, int wpWidth, int wpHeight, Camera* cam);
    void draw();
    void save();

    void load(const char* pFilename);

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
