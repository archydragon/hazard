#include <iostream>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Texture.h"

Texture::Texture() = default;

bool Texture::init() {
    if (ISceneObject::init()) {
        return true;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID); // all upcoming GL_TEXTURE_2D operations now have
                                             // effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
        GL_CLAMP_TO_EDGE); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int tWidth, tHeight, nrChannels;
    unsigned char* texData = stbi_load(filename.c_str(), &tWidth, &tHeight, &nrChannels, 0);
    assert(texData != NULL);

    if (nrChannels == 3) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tWidth, tHeight, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     texData);
    } else if (nrChannels == 4) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tWidth, tHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     texData);
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(texData);

    std::cout << "Texture file " << filename << " loaded." << std::endl;
    return true;
}
