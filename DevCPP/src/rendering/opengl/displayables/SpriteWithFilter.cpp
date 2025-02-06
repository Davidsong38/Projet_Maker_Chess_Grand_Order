//
// Created by sebas on 06/02/2025.
//

#include "SpriteWithFilter.h"

#include <iostream>
#include <log.h>
#include <ostream>
#include <ext/matrix_transform.hpp>

#include "vaos.h"
#include "glFunctions.h"
#include "RenderEngine.h"

void SpriteWithFilter::draw() {
    glm::vec3 filterColor = glm::vec3();
    glm::vec4 defaultColor = glm::vec4();
    if (this->spriteTarget != nullptr) {
        this->position.x = spriteTarget->getSpriteX();
        this->position.y = spriteTarget->getSpriteY();
        this->rotation = spriteTarget->getSpriteRotation();
        filterColor = spriteTarget->getFilterColor();
        defaultColor = spriteTarget->getDefaultColor();
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
    this->shader->setVec3("filterColor", filterColor);
    this->shader->setVec4("defaultColor", defaultColor);
    glDrawArrays(GL_TRIANGLES, 0, VERTEX_PER_QUAD);
}