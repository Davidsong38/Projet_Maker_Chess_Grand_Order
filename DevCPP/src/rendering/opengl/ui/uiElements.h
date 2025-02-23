//
// Created by sebas on 25/01/2025.
//

#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include <SpriteGroup.h>

#include "Sprite.h"

#include <vector>

extern std::vector<Displayable*> ui_sprites;

void initUIElements();
void addAdditionalUIElement(Displayable* sprite);
void addAdditionalUIElement(Texture* texture, glm::vec2 size, SpriteTarget* target);

void loadPossibleMoves(const std::vector<std::pair<int, int>>& possible_moves, glm::vec4 color);
void unloadPossibleMoves();

bool compareRenderOrder(Displayable* a, Displayable* b);

#endif //UIELEMENTS_H
