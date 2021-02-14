#ifndef HAZARD_IDRAWABLE_H
#define HAZARD_IDRAWABLE_H

#include <glm/glm.hpp>

enum DrawableType
{
    UNKNOWN,
    CUBE  = 1001,
    PLANE = 1002,
};

class IDrawable {
public:
    virtual ~IDrawable() = default;
    virtual const char* icon() {
        return "";
    };
    virtual void init(){};
    virtual unsigned int draw(glm::mat4 projection, glm::mat4 view) {
        return 0;
    };
};

#endif // HAZARD_IDRAWABLE_H
