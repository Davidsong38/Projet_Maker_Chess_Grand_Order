//
// Created by sebas on 01/02/2025.
//

#ifndef SPRITETARGET_H
#define SPRITETARGET_H
#include "glm.hpp"


class SpriteTarget {
public:
  virtual ~SpriteTarget() = default;
  virtual float getSpriteX() = 0;
  virtual float getSpriteY() = 0;
  virtual glm::vec3 getFilterColor() = 0;
  virtual glm::vec4 getDefaultColor() = 0;
  virtual float getSpriteRotation() = 0;
  virtual bool isHidden() = 0;
};



#endif //SPRITETARGET_H
