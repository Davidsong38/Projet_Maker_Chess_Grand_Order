//
// Created by sebas on 24/01/2025.
//

#ifndef SPRITE_H
#define SPRITE_H

#include <Texture.h>
#include <VAO.h>
#include <Shader.h>

#include "Displayable.h"
#include "SpriteTarget.h"

class Sprite : public Displayable {
public:
    Sprite(Texture* texture, glm::vec3 position, glm::vec2 size);
    Sprite(Texture* texture, glm::vec2 size, SpriteTarget* target);
    Sprite(Texture* texture, glm::vec2 size, SpriteTarget* target, float layer);
    void draw() override;
    void load() override;
    void unload() override;
    float getDistanceToCamera() override { return -position.z; }
protected:
    VAO* vao;
    Texture* texture;
    Shader* shader;
    glm::vec3 position{};
    glm::vec2 size{};
    float rotation{};
    SpriteTarget* spriteTarget;
};



#endif //SPRITE_H
