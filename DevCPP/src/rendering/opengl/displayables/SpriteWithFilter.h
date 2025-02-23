//
// Created by sebas on 06/02/2025.
//

#ifndef SPRITEWITHFILTER_H
#define SPRITEWITHFILTER_H

#include <shaders.h>
#include <Sprite.h>


class SpriteWithFilter : Sprite {
public:
    SpriteWithFilter(Texture* texture, glm::vec2 size, SpriteTarget* target) : Sprite(texture, size, target) {
        this->shader = basicSpriteShaderWithFilter;
    };
    SpriteWithFilter(Texture* texture, glm::vec2 size, SpriteTarget* target, const float layer) : Sprite(texture, size, target, layer) {
        this->shader = basicSpriteShaderWithFilter;
    };
    void draw() override;
};



#endif //SPRITEWITHFILTER_H
