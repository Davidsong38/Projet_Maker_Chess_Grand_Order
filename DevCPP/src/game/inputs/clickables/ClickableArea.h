//
// Created by sebas on 19/02/2025.
//

#ifndef CLICKABLEAREA_H
#define CLICKABLEAREA_H

#include <glm.hpp>
#include <Texture.h>
#include <SpriteWithFilter.h>
#include <functional>

class ClickableArea : public SpriteTarget {
public:
  ClickableArea(Texture* texture, glm::vec3 position, glm::vec2 size, glm::vec3 pressedFilter, std::function<bool(glm::vec2)> on_click);
  ~ClickableArea() override;

  bool clickAt(glm::vec2 position);
  void releaseClick();
  [[nodiscard]] float getDistanceToCamera() const { return -position.z; }

  void hide();
  void show();
  void toggle();

  virtual void updatePressed(bool clickReceived) { this->pressed = false; }

  float getSpriteX() override;
  float getSpriteY() override;
  glm::vec3 getFilterColor() override;
  glm::vec4 getDefaultColor() override;
  float getSpriteRotation() override;
  bool isHidden() override;
protected:
  [[nodiscard]] glm::vec2 getPosInBox(glm::vec2 pos) const;
  SpriteWithFilter* sprite;
  glm::vec3 position{};
  glm::vec2 size{1, 1};
  bool pressed = false;
  bool hidden = false;
  glm::vec3 pressedFilter{};
  std::function<bool(glm::vec2)> on_click;
};



#endif //CLICKABLEAREA_H
