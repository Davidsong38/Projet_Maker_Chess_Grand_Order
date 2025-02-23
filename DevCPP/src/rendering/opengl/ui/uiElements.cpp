//
// Created by sebas on 25/01/2025.
//

#include "uiElements.h"

#include <game_cfg.h>
#include <SpriteGroup.h>
#include <SpriteWithFilter.h>

#include "textures.h"
#include "rendering_cfg.h"
#include "RenderEngine.h"
#include "clickables.h"

std::vector<Displayable*> ui_sprites;
SpriteGroup* possible_moves_sprite_group;

void initUIElements() {
  ui_sprites.push_back((Displayable*) new Sprite(
    backgroundTexture,
    glm::vec3(0, 0, LAYER_BACKGROUND),
    glm::vec2(1, 1)
  ));

  possible_moves_sprite_group = new SpriteGroup(
    emptyTexture,
    BOARD_SIZE * BOARD_SIZE
  );

  ui_sprites.push_back((Displayable*) possible_moves_sprite_group);

  initClickables();
}

void addAdditionalUIElement(Displayable* sprite) {
  ui_sprites.push_back(sprite);
  std::sort(ui_sprites.begin(), ui_sprites.end(), compareRenderOrder);
}

void addAdditionalUIElement(Texture* texture, const glm::vec2 size, SpriteTarget* target) {
  addAdditionalUIElement(reinterpret_cast<Displayable *>(new SpriteWithFilter(texture, size, target)));
}

void loadPossibleMoves(const std::vector<std::pair<int, int>>& possible_moves, const glm::vec4 color) {
  for (const auto&[fst, snd] : possible_moves) {
    spritePositioner_type sprite_positioner{};
    constexpr float offset = 1.0f / BOARD_SIZE;
    constexpr float offset_2 = 2.0f / BOARD_SIZE;
    sprite_positioner.position = glm::vec3(offset - 1.0f, 1.0f - offset, LAYER_GROUND);
    sprite_positioner.position.x += offset_2 * static_cast<float>(snd);
    sprite_positioner.position.x *= RenderEngine::getWindowInverseAspectRatio();
    sprite_positioner.position.y -= offset_2 * static_cast<float>(fst);
    sprite_positioner.size = glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE);
    sprite_positioner.defaultColor = color;
    possible_moves_sprite_group->addPosition(sprite_positioner);
  }
}

void unloadPossibleMoves() {
  possible_moves_sprite_group->clearPositions();
}

bool compareRenderOrder(Displayable* a, Displayable* b) {
  return a->getDistanceToCamera() < b->getDistanceToCamera();
}
