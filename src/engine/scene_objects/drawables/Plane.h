#ifndef HAZARD_PLANE_H
#define HAZARD_PLANE_H

#include "IDrawable.h"

class Plane : public IDrawable {
public:
    const char* icon() override {
        return "\xee\x83\x8b";
    }

    void init() override {
        // clang-format off
        float vertices[] = {
            -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
             1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
             1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -1.0f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -1.0f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f
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
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    unsigned int draw(glm::mat4 projection, glm::mat4 view) override {
        unsigned int drawCalls = 0;

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        drawCalls++;
        glBindVertexArray(0);

        return drawCalls;
    }

private:
    unsigned int vao = 0;
    unsigned int vbo = 0;
};

#endif // HAZARD_PLANE_H
