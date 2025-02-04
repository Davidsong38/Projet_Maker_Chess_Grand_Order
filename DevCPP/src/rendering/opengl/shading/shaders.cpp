//
// Created by sebas on 10/01/2025.
//

#include "shaders.h"

Shader* postProcessingShader;
Shader* skyboxShader;
Shader* basicSpriteShader;

void initShaders() {
  postProcessingShader = new Shader("postProcessing");
  skyboxShader = new Shader("skybox");
  basicSpriteShader = new Shader("basicSprite");
}