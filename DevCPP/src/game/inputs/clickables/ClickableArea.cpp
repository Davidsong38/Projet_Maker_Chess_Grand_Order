//
// Created by sebas on 19/02/2025.
//

#include "ClickableArea.h"

#include <utility>
#include "uiElements.h"


ClickableArea::ClickableArea(Texture* texture, const glm::vec3 position, const glm::vec2 size, const glm::vec3 pressedFilter, std::function<bool(glm::vec2)> on_click) {
    this->position = position;
    this->size = size;
    this->pressedFilter = pressedFilter;
    this->on_click = std::move(on_click);
    this->hidden = false;
    if (texture != nullptr) {
        this->sprite = new SpriteWithFilter(
            texture,
            size,
            this,
            position.z
        );
        addAdditionalUIElement(reinterpret_cast<Displayable *>(this->sprite));
    }
}

ClickableArea::~ClickableArea() {
    delete this->sprite;
}

bool ClickableArea::clickAt(const glm::vec2 position) {
    if (this->hidden) return false;
    const glm::vec2 transformed_position = getPosInBox(position);
    if (
        transformed_position.y < 0
        || transformed_position.y >= 1
        || transformed_position.x < 0
        || transformed_position.x >= 1
    ) {
        return false;
    }
    updatePressed(true);
    return on_click(transformed_position);
}

void ClickableArea::releaseClick() {
    updatePressed(false);
}

void ClickableArea::hide() {
    this->hidden = true;
}

void ClickableArea::show() {
    this->hidden = false;
}

void ClickableArea::toggle() {
    this->hidden = !this->hidden;
}

float ClickableArea::getSpriteX() {
    return this->position.x;
}

float ClickableArea::getSpriteY() {
    return this->position.y;
}

glm::vec3 ClickableArea::getFilterColor() {
    if (pressed)
        return this->pressedFilter;
    return {};
}

glm::vec4 ClickableArea::getDefaultColor() {
    return {};
}

float ClickableArea::getSpriteRotation() {
    return 0.0f;
}

bool ClickableArea::isHidden() {
    return this->hidden;
}

glm::vec2 ClickableArea::getPosInBox(const glm::vec2 pos) const {
    return {
        ( pos.x - this->position.x + this->size.x) * 0.5f / this->size.x,
        (-pos.y - this->position.y + this->size.y) * 0.5f / this->size.y
    };
}
