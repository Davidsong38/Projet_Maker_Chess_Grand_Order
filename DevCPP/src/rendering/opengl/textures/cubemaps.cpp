//
// Created by sebas on 12/01/2025.
//

#include "cubemaps.h"

CubeMap* skybox;

void loadCubeMaps() {
  GLuint cubemap_ids[CM_COUNT];
  glGenTextures(CM_COUNT, cubemap_ids);

  skybox = new CubeMap(cubemap_ids[CM_SKYBOX], "skybox");
}