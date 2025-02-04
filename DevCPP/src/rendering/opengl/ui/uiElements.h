//
// Created by sebas on 25/01/2025.
//

#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include "Sprite.h"

#include <vector>

enum ui_sprite_type {
    UI_SPRITE_BACKGROUND,
    UI_SPRITE_BOARD,
    UI_SPRITE_COUNT
};

extern Sprite** ui_sprites;
extern std::vector<Sprite*> ui_additional_sprites;

void initUIElements();
void addAdditionalUIElement(Sprite* sprite);
void addAdditionalUIElement(Texture* texture, glm::vec2 size, SpriteTarget* target);

#endif //UIELEMENTS_H
