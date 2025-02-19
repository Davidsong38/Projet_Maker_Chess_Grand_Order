//
// Created by sebas on 25/01/2025.
//

#include "uiElements.h"

#include <iostream>
#include <ostream>
#include <SpriteGroup.h>
#include <SpriteWithFilter.h>

#include "textures.h"
#include "rendering_cfg.h"
#include "RenderEngine.h"

Displayable** ui_sprites;
std::vector<Sprite*> ui_additional_sprites;
SpriteGroup* possible_moves_sprite_group;

void initUIElements() {
  ui_sprites = static_cast<Displayable**>(malloc(sizeof(Displayable *) * UI_SPRITE_COUNT));

  ui_sprites[UI_SPRITE_BACKGROUND] = (Displayable*) new Sprite(
    backgroundTexture,
    glm::vec3(0, 0, LAYER_BACKGROUND),
    glm::vec2(1, 1)
  );

  ui_sprites[UI_SPRITE_BOARD] = (Displayable*) new Sprite(
    boardTexture,
    glm::vec3(0, 0, LAYER_BOARD),
    glm::vec2(RenderEngine::getWindowInverseAspectRatio(), 1)
  );

  possible_moves_sprite_group = new SpriteGroup(
    emptyTexture,
    64
  );

  ui_sprites[UI_SPRITE_POSSIBLE_MOVES] = (Displayable*) possible_moves_sprite_group;
}

void addAdditionalUIElement(Sprite* sprite) {
  ui_additional_sprites.push_back(sprite);
}

void addAdditionalUIElement(Texture* texture, glm::vec2 size, SpriteTarget* target) {
  addAdditionalUIElement(reinterpret_cast<Sprite *>(new SpriteWithFilter(texture, size, target)));
}

void loadPossibleMoves(const std::vector<std::pair<int, int>>& possible_moves, glm::vec4 color) {
  for (const auto& possible_move : possible_moves) {
    spritePositioner_type sprite_positioner{};
    sprite_positioner.position = glm::vec3(-0.875f, 0.875f, LAYER_GROUND);
    sprite_positioner.position.x += 0.25f * static_cast<float>(possible_move.second);
    sprite_positioner.position.x *= RenderEngine::getWindowInverseAspectRatio();
    sprite_positioner.position.y -= 0.25f * static_cast<float>(possible_move.first);
    sprite_positioner.size = glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE);
    sprite_positioner.defaultColor = color;
    possible_moves_sprite_group->addPosition(sprite_positioner);
  }
}

void unloadPossibleMoves() {
  possible_moves_sprite_group->clearPositions();
}