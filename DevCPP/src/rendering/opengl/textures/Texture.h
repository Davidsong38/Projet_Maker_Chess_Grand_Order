//
// Created by sebas on 24/01/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include "glm.hpp"

class Texture {
public:
    Texture(unsigned int textureID, glm::ivec2 size, unsigned int format);
    Texture(unsigned int textureID, const std::string &textureName, unsigned int format);
    void bind() const;
    void useAsTarget() const;
private:
    unsigned int textureID;
    int width{0}, height{0}, nrChannels{0};
};



#endif //TEXTURE_H
