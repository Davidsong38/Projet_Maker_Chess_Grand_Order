//
// Created by sebas on 13/02/2025.
//

#ifndef SPRITEPOSITIONER_H
#define SPRITEPOSITIONER_H

#include <glm.hpp>

struct spritePositioner_type {
  glm::vec3 position;
  glm::vec4 defaultColor;
  glm::vec3 filterColor;
  glm::vec2 size;
  float rotation;
};

#endif //SPRITEPOSITIONER_H