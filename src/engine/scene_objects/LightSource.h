#ifndef HAZARD_LIGHTSOURCE_H
#define HAZARD_LIGHTSOURCE_H

#include <glm/glm.hpp>
#include <glm2json.h>
#include <nlohmann/json.hpp>

#include "ISceneObject.h"
#include "ShaderProgram.h"

class LightSource : public ISceneObject {
public:
    LightSource();
    LightSource(ObjectID id, const char* name);

    const char* icon() override {
        return "\xee\x83\x8e";
    }
    void resolveLinks(const Objects& objs) override;
    void init() override;
    void prepareDepthMap() const;
    void useShader(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
    void cleanup();

    ObjectType type = LIGHT_SOURCE;

    glm::vec3 position  = glm::vec3(0, 0, 0);
    glm::vec3 direction = glm::vec3(1, 0, -1);

    const unsigned int shadowMapSize = 4096;
    glm::mat4 lightSpace;
    unsigned int depthMap = 0;

private:
    ShaderProgram* shader    = nullptr;
    unsigned int depthMapFBO = 0;
};

// Macros for JSON (de)serialization.
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(LightSource, id, type, name, links, position, direction)

#endif // HAZARD_LIGHTSOURCE_H
