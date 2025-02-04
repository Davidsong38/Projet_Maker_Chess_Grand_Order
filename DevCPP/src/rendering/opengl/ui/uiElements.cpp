//
// Created by sebas on 25/01/2025.
//

#include "uiElements.h"
#include "textures.h"

Sprite** ui_sprites;

void initUIElements() {
  ui_sprites = static_cast<Sprite **>(malloc(sizeof(Sprite *) * UI_SPRITE_COUNT));

  ui_sprites[UI_SPRITE_BACKGROUND] = new Sprite(
    backgroundTexture,
    glm::vec3(0, 0, -0.1),
    glm::vec2(1, 1)
  );

  ui_sprites[UI_SPRITE_BOARD] = new Sprite(
    boardTexture,
    glm::vec3(0, 0, -0.3),
    glm::vec2(1080.0/1920.0, 1)
  );
}