//
// Created by sebas on 24/01/2025.
//

#include "Texture.h"
#include "stb_image.h"

Texture::Texture(const GLuint textureID, const glm::ivec2 size, const GLuint format) {
    this->textureID = textureID;
    this->width = size.x;
    this->height = size.y;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        nullptr
    );
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::Texture(const GLuint textureID, const std::string &textureName, const GLuint format) {
    // Load texture
    std::string path = "assets/textures/" + textureName;
    stbi_set_flip_vertically_on_load(true);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    // Gen opengl stuff
    this->textureID = textureID;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width,
        height,
        0,
        format,
        GL_UNSIGNED_BYTE,
        data
    );
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::useAsTarget() const {
    glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        textureID,
        0
    );
}