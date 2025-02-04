//
// Created by sebas on 12/01/2025.
//

#ifndef CUBEMAPS_H
#define CUBEMAPS_H

#include "CubeMap.h"

enum cubemap_type{
  CM_SKYBOX,
  CM_COUNT
};

extern CubeMap* skybox;

void loadCubeMaps();

#endif //CUBEMAPS_H
