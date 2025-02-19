//
// Created by sebas on 09/02/2025.
//

#include "SpriteGroup.h"

#include <iostream>
#include <ostream>

#include "vaos.h"
#include "shaders.h"
#include "glFunctions.h"
#include "RenderEngine.h"
#include "rendering_cfg.h"
#include <ext/matrix_transform.hpp>


SpriteGroup::SpriteGroup(Texture* texture, int positionsSize) {
    this->vao = quadVAO;
    this->texture = texture;
    this->shader = basicSpriteShaderWithFilter;
    this->positionsSize = positionsSize;
    this->spritePositioners = static_cast<spritePositioner_type *>(malloc(positionsSize * sizeof(spritePositioner_type)));
}

SpriteGroup::~SpriteGroup() {
    free(spritePositioners);
}

void SpriteGroup::draw() {
    this->vao->bind();
    this->texture->bind();
    this->shader->use();
    for (int i = 0; i < this->positionCount; i++) {
        const spritePositioner_type* sp = &this->spritePositioners[i];

        auto model = glm::mat4(1.0f);
        model = translate(model, sp->position);
        model = scale(model, glm::vec3(sp->size.x, sp->size.y, 1));
        model = rotate(model, glm::radians(sp->rotation), glm::vec3(0, 0, 1));
        this->shader->setMat4("model", model);
        this->shader->setVec4("defaultColor", sp->defaultColor);
        this->shader->setVec3("filterColor", sp->filterColor);
        glDrawArrays(GL_TRIANGLES, 0, VERTEX_PER_QUAD);
    }
}

void SpriteGroup::load() {

}

void SpriteGroup::unload() {

}

void SpriteGroup::addPosition(const spritePositioner_type position) {
    spritePositioners[positionCount++] = position;
}

void SpriteGroup::clearPositions() {
    positionCount = 0;
}
