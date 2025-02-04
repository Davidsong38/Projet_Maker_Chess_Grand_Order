//
// Created by sebas on 24/01/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "glFunctions.h"
#include <string>
#include "glm.hpp"

class Texture {
public:
    Texture(GLuint textureID, glm::ivec2 size, GLuint format);
    Texture(GLuint textureID, const std::string &textureName, GLuint format);
    void bind() const;
    void useAsTarget() const;
private:
    GLuint textureID;
    int width{0}, height{0}, nrChannels{0};
};



#endif //TEXTURE_H
