#include <glad/glad.h>

#include "LightSource.h"

LightSource::LightSource() = default;

LightSource::LightSource(ObjectID id, const char* name) : ISceneObject(id, name) {
    registerLinkName("shaderProgramID");
}

void LightSource::resolveLinks(const Objects& objs) {
    ISceneObject::resolveLinks(objs);

    if (links["shaderProgramID"] > 0) {
        shader = (ShaderProgram*)objs.at(links["shaderProgramID"]).get();
    } else {
        shader = nullptr;
    }
}

void LightSource::init() {
    ISceneObject::init();

    glGenFramebuffers(1, &depthMapFBO);
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapSize, shadowMapSize, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightSource::prepareDepthMap() const {
    glViewport(0, 0, shadowMapSize, shadowMapSize);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void LightSource::useShader(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
    shader->use();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);
    shader->setMat4("model", model);
}

void LightSource::cleanup() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
