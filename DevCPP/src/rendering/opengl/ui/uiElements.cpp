//
// Created by sebas on 25/01/2025.
//

#include "uiElements.h"

#include <Chessboard.h>
#include <game_cfg.h>
#include <SpriteGroup.h>
#include <SpriteWithFilter.h>

#include "textures.h"
#include "rendering_cfg.h"
#include "RenderEngine.h"
#include "clickables.h"

std::vector<Displayable*> ui_sprites;
SpriteGroup* possible_moves_sprite_group;
SpriteGroup* selection_sprite_group;

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

  selection_sprite_group = new SpriteGroup(
    emptyTexture,
    BOARD_SIZE * BOARD_SIZE
  );

  ui_sprites.push_back((Displayable*) possible_moves_sprite_group);
  ui_sprites.push_back((Displayable*) selection_sprite_group);

  initClickables();
}

void addAdditionalUIElement(Displayable* sprite) {
  ui_sprites.push_back(sprite);
  std::sort(ui_sprites.begin(), ui_sprites.end(), compareRenderOrder);
}

void addAdditionalUIElement(Texture* texture, const glm::vec2 size, SpriteTarget* target) {
  addAdditionalUIElement(reinterpret_cast<Displayable *>(new SpriteWithFilter(texture, size, target)));
}

void loadPossibleMoves(const std::vector<glm::ivec2>& possible_moves, const glm::vec4 color) {
  for (const auto& possible_move : possible_moves) {
    spritePositioner_type sprite_positioner{};
    constexpr float offset = 1.0f / BOARD_SIZE;
    constexpr float offset_2 = 2.0f / BOARD_SIZE;
    sprite_positioner.position = glm::vec3(offset - 1.0f, 1.0f - offset, LAYER_GROUND);
    sprite_positioner.position.x += offset_2 * static_cast<float>(possible_move.y);
    sprite_positioner.position.x *= RenderEngine::getWindowInverseAspectRatio();
    sprite_positioner.position.y -= offset_2 * static_cast<float>(possible_move.x);
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

void updateSelectionSpriteGroup() {
  selection_sprite_group->clearPositions();
  for (int x = 0; x < BOARD_SIZE; x++) {
    for (int y = 0; y < BOARD_SIZE; y++) {
      const auto* cell = Chessboard::getInstance()->getCellAt(x, y);
      if (cell->selected == false)
        continue;
      spritePositioner_type sprite_positioner{};
      constexpr float offset = 1.0f / BOARD_SIZE;
      constexpr float offset_2 = 2.0f / BOARD_SIZE;
      sprite_positioner.position = glm::vec3(offset - 1.0f, 1.0f - offset, LAYER_GROUND);
      sprite_positioner.position.x += offset_2 * static_cast<float>(y);
      sprite_positioner.position.x *= RenderEngine::getWindowInverseAspectRatio();
      sprite_positioner.position.y -= offset_2 * static_cast<float>(x);
      sprite_positioner.size = glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE);
      if (cell->piece == nullptr)
        sprite_positioner.defaultColor = glm::vec4(0.9f, 0.9f, 0.4f, 0.5f);
      else if (cell->piece->getIsWhite())
        sprite_positioner.defaultColor = glm::vec4(0.25f, 0.875f, 1.0f, 0.3f);
      else
        sprite_positioner.defaultColor = glm::vec4(1.0f, 0.3f, 0.3f, 0.3f);
      selection_sprite_group->addPosition(sprite_positioner);
    }
  }
}
