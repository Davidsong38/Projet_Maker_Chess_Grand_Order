//
// Created by sebas on 25/01/2025.
//

#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include <SpriteGroup.h>

#include "Sprite.h"

#include <vector>

enum ui_sprite_type {
    UI_SPRITE_BACKGROUND,
    UI_SPRITE_BOARD,
    UI_SPRITE_POSSIBLE_MOVES,
    UI_SPRITE_COUNT
};

extern Displayable** ui_sprites;
extern std::vector<Sprite*> ui_additional_sprites;

void initUIElements();
void addAdditionalUIElement(Sprite* sprite);
void addAdditionalUIElement(Texture* texture, glm::vec2 size, SpriteTarget* target);

void loadPossibleMoves(const std::vector<std::pair<int, int>>& possible_moves, glm::vec4 color);
void unloadPossibleMoves();

#endif //UIELEMENTS_H
