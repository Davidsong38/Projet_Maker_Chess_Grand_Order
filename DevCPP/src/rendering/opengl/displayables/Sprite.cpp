//
// Created by sebas on 24/01/2025.
//

#include "Sprite.h"

#include <iostream>
#include <log.h>
#include <ostream>
#include <ext/matrix_transform.hpp>

#include "vaos.h"
#include "shaders.h"
#include "glFunctions.h"
#include "RenderEngine.h"
#include "rendering_cfg.h"


Sprite::Sprite(Texture* texture, const glm::vec3 position, const glm::vec2 size) {
    this->vao = quadVAO;
    this->texture = texture;
    this->shader = basicSpriteShader;
    this->position = position;
    this->size = size;
    this->spriteTarget = nullptr;
}

Sprite::Sprite(Texture* texture, const glm::vec2 size, SpriteTarget* target) {
    this->vao = quadVAO;
    this->texture = texture;
    this->shader = basicSpriteShader;
    this->position = glm::vec3(0,0,LAYER_PIECES);
    this->size = size;
    this->spriteTarget = target;
}

Sprite::Sprite(Texture* texture, const glm::vec2 size, SpriteTarget* target, const float layer) {
    this->vao = quadVAO;
    this->texture = texture;
    this->shader = basicSpriteShader;
    this->position = glm::vec3(0,0,layer);
    this->size = size;
    this->spriteTarget = target;
}

void Sprite::draw() {
    if (this->spriteTarget != nullptr) {
        this->position.x = spriteTarget->getSpriteX();
        this->position.y = spriteTarget->getSpriteY();
        this->rotation = spriteTarget->getSpriteRotation();
        if (spriteTarget->isHidden())
            return;
    }
    this->vao->bind();
    this->texture->bind();
    this->shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = translate(model, this->position);
    model = scale(model, glm::vec3(this->size.x, this->size.y, 1));
    model = rotate(model, glm::radians(this->rotation), glm::vec3(0, 0, 1));
    this->shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, VERTEX_PER_QUAD);
}

void Sprite::load() {

}

void Sprite::unload() {

}