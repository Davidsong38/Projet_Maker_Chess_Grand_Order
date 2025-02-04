//
// Created by sebas on 12/01/2025.
//

#ifndef VAOS_H
#define VAOS_H

#include "VAO.h"

enum vao_type{
  VAO_QUAD,
  VAO_CUBE,
  VAO_SKYBOX,
  VAO_COUNT
};

extern VAO* quadVAO;
extern VAO* cubeVAO;
extern VAO* skyboxVAO;

void initVAOs();



#endif //VAOS_H
