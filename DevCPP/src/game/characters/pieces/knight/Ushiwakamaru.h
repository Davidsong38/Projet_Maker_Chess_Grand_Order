//
// Created by david on 31/01/2025.
//

#ifndef USHIWAKAMARU_H
#define USHIWAKAMARU_H

#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Pieces.h"


class Ushiwakamaru final : public Knight {
protected:
    const int cooldown{3};
    int remaining_cooldown{0};
public:
    Ushiwakamaru(const int startX, const int startY, const bool white, const Characters_List hero)
    : Knight(startX, startY, white, hero)  {
        this->default_piece_move = ushiwakamaru_moves;
        addAdditionalUIElement(
            ushiwakamaruTexture,
            glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
            this
        );
    }

    bool canEvolve() override;
    bool evolvedForm() override;
    bool SpellActivationCheck() override;
};



#endif //USHIWAKAMARU_H
