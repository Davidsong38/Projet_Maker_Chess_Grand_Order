//
// Created by david on 31/01/2025.
//

#ifndef NEMO_MARINE_H
#define NEMO_MARINE_H

#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Pieces.h"


class Nemo_Marine final: public Pawn  {
public:
Nemo_Marine(const int startX, const int startY, const bool white, const Characters_List hero)
    : Pawn(startX, startY, white, hero)  {
        addAdditionalUIElement(
            nemoMarxTexture,
            glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
            this
        );
    }
};

#endif //NEMO_MARINE_H
