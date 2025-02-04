//
// Created by sebas on 25/01/2025.
//

#ifndef UIELEMENTS_H
#define UIELEMENTS_H

#include "Sprite.h"

enum ui_sprite_type {
    UI_SPRITE_BACKGROUND,
    UI_SPRITE_BOARD,
    UI_SPRITE_COUNT
};

extern Sprite** ui_sprites;

void initUIElements();

#endif //UIELEMENTS_H
