//
// Created by sebas on 09/01/2025.
//

#ifndef SHADERS_H
#define SHADERS_H

#include "Shader.h"

extern Shader* postProcessingShader;
extern Shader* skyboxShader;
extern Shader* basicSpriteShader;
extern Shader* basicSpriteShaderWithFilter;

void initShaders();

#endif //SHADERS_H
