//
// Created by sebas on 19/02/2025.
//

#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <ClickableArea.h>
#include <utility>


class CheckBox : ClickableArea {
public:
    CheckBox(Texture* texture, const glm::vec3 position, const glm::vec2 size, const glm::vec3 pressedFilter, std::function<bool(glm::vec2)> on_click)
        : ClickableArea(texture, position, size, pressedFilter, std::move(on_click)) {};
    void updatePressed(bool clickReceived) override;
};



#endif //CHECKBOX_H
