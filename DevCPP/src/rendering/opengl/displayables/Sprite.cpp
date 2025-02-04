//
// Created by sebas on 24/01/2025.
//

#include "Sprite.h"

#include <ext/matrix_transform.hpp>

#include "vaos.h"
#include "shaders.h"
#include "glFunctions.h"
#include "RenderEngine.h"


Sprite::Sprite(Texture* texture, const glm::vec3 position, const glm::vec2 size) {
    this->vao = quadVAO;
    this->texture = texture;
    this->shader = basicSpriteShader;
    this->position = position;
    this->size = size;
    this->target = nullptr;
}

void Sprite::draw() {
    if (this->target != nullptr) {
        this->position.x = target->getSpriteX();
        this->position.y = target->getSpriteY();
    }
    this->vao->bind();
    this->texture->bind();
    this->shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = translate(model, this->position);
    model = scale(model, glm::vec3(this->size.x, this->size.y, 1));
    this->shader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, VERTEX_PER_QUAD);
}

void Sprite::load() {

}

void Sprite::unload() {

}