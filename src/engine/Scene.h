#ifndef HAZARD_SCENE_H
#define HAZARD_SCENE_H

#include "Camera.h"
#include "Shader.h"

class Scene {
public:
    Scene(int wpWidth, int wpHeight, Camera* cam);
    void draw();

private:
    int screenWidth;
    int screenHeight;
    Camera* camera;
    Shader* shader;
    unsigned int vao = 0;
    unsigned int vbo = 0;
};

#endif // HAZARD_SCENE_H
