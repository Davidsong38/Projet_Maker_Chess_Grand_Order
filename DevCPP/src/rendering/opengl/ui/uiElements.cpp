//
// Created by sebas on 25/01/2025.
//

#include "uiElements.h"

#include <SpriteWithFilter.h>

#include "textures.h"
#include "rendering_cfg.h"
#include "RenderEngine.h"

Sprite** ui_sprites;
std::vector<Sprite*> ui_additional_sprites;

void initUIElements() {
  ui_sprites = static_cast<Sprite**>(malloc(sizeof(Sprite *) * UI_SPRITE_COUNT));

  ui_sprites[UI_SPRITE_BACKGROUND] = new Sprite(
    backgroundTexture,
    glm::vec3(0, 0, LAYER_BACKGROUND),
    glm::vec2(1, 1)
  );

  ui_sprites[UI_SPRITE_BOARD] = new Sprite(
    boardTexture,
    glm::vec3(0, 0, LAYER_BOARD),
    glm::vec2(RenderEngine::getWindowInverseAspectRatio(), 1)
  );
}

void addAdditionalUIElement(Sprite* sprite) {
  ui_additional_sprites.push_back(sprite);
}

void addAdditionalUIElement(Texture* texture, glm::vec2 size, SpriteTarget* target) {
  addAdditionalUIElement(reinterpret_cast<Sprite *>(new SpriteWithFilter(texture, size, target)));
}