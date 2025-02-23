//
// Created by sebas on 19/02/2025.
//

#include "RadioButton.h"

#include <clickables.h>
#include <log.h>

#include "uiElements.h"

RadioButton::RadioButton(
        const std::vector<Texture *> &buttonTextures,
        const glm::vec3 position,
        const glm::vec2 button_size,
        const glm::vec2 button_spacing,
        Texture * selection_texture,
        const glm::vec2 selection_size,
        const glm::vec3 selection_offset,
        const glm::vec3 button_filter_color
) {
    this->selection_offset = selection_offset;
    this->selectedRadioButtonIndex = -1;
    this->hidden = false;
    const int nb_buttons = static_cast<int>(buttonTextures.size());
    for (int i = 0; i < nb_buttons; i++) {
        const auto button_position = glm::vec3(
            position.x + static_cast<float>(i) * button_spacing.x,
            position.y + static_cast<float>(i) * button_spacing.y,
            position.z
        );
        const auto parent = this;
        auto* button = new Button(
            buttonTextures[i],
            button_position,
            button_size,
            button_filter_color,
            [parent, i](const glm::vec2 /*pos*/) {
                parent->setIndex(i);
                return true;
            }
        );
        this->radioButtonButtons.push_back(button);
        addClickableAreaLMB(button);
    }
    this->selection_sprite = new Sprite(
        selection_texture,
        selection_size,
        this,
        selection_offset.z
    );
    addAdditionalUIElement(this->selection_sprite);
}

RadioButton::RadioButton(
        const std::vector<Texture *> &buttonTextures,
        const glm::vec3 position,
        const glm::vec2 button_size,
        const glm::vec2 button_spacing,
        Texture * selection_texture,
        const glm::vec2 selection_size,
        const glm::vec3 selection_offset,
        const glm::vec3 button_filter_color,
        bool centered
) {
    this->selection_offset = selection_offset;
    this->selectedRadioButtonIndex = -1;
    this->hidden = false;
    const int nb_buttons = static_cast<int>(buttonTextures.size());
    for (int i = 0; i < nb_buttons; i++) {
        auto button_position = glm::vec3(0, 0, position.z);
        if (centered) {
            auto start_position = glm::vec2(
                position.x + static_cast<float>(buttonTextures.size() - 1) * button_spacing.x * 0.5f,
                position.y - static_cast<float>(buttonTextures.size() - 1) * button_spacing.y * 0.5f
            );
            button_position.x = start_position.x + static_cast<float>(i) * button_spacing.x;
            button_position.y = start_position.y + static_cast<float>(i) * button_spacing.y;
        } else {
            button_position.x = position.x + static_cast<float>(i) * button_spacing.x;
            button_position.y = position.y + static_cast<float>(i) * button_spacing.y;
        }
        const auto parent = this;
        auto* button = new Button(
            buttonTextures[i],
            button_position,
            button_size,
            button_filter_color,
            [parent, i](const glm::vec2 /*pos*/) {
                parent->setIndex(i);
                return true;
            }
        );
        this->radioButtonButtons.push_back(button);
        addClickableAreaLMB(button);
    }
    this->selection_sprite = new Sprite(
        selection_texture,
        selection_size,
        this,
        selection_offset.z
    );
    addAdditionalUIElement(this->selection_sprite);
}

void RadioButton::setIndex(const int index) {
    if (index < radioButtonButtons.size() && index >= 0) {
        this->selectedRadioButtonIndex = index;
    } else {
        ltr_log_error("RadioButton::setIndex: index out of range : ", index);
    }
}

int RadioButton::getIndex() const {
    return this->selectedRadioButtonIndex;
}

void RadioButton::hide() {
    this->hidden = true;
    for (auto* button : radioButtonButtons) {
        button->hide();
    }
}

void RadioButton::show() {
    this->hidden = false;
    for (auto* button : radioButtonButtons) {
        button->show();
    }
}

float RadioButton::getSpriteX() {
    if (this->selectedRadioButtonIndex == -1)
        return 0;
    return this->selection_offset.x + this->radioButtonButtons[this->selectedRadioButtonIndex]->getSpriteX();
}

float RadioButton::getSpriteY() {
    if (this->selectedRadioButtonIndex == -1)
        return 0;
    return this->selection_offset.y + this->radioButtonButtons[this->selectedRadioButtonIndex]->getSpriteY();
}

glm::vec3 RadioButton::getFilterColor() {
    return {};
}

glm::vec4 RadioButton::getDefaultColor() {
    return {};
}

float RadioButton::getSpriteRotation() {
    return 0;
}

bool RadioButton::isHidden() {
    return this->hidden || this->selectedRadioButtonIndex == -1;
}

