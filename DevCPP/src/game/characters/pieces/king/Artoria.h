//
// Created by david on 31/01/2025.
//

#ifndef ARTORIA_H
#define ARTORIA_H

#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Pieces.h"


class Artoria final: public King{
protected :
    int CNT_GodMove = 0;
public:
Artoria(const int startX, const int startY, const bool white, const Characters_List hero)
    : King(startX, startY, white, hero)  {
    defaultEffectsRanges[AOE] = [this](){return this->getEffectRange(AOE);};
        addAdditionalUIElement(
            artoriaTexture,
            glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
            this
        );
    }

    [[nodiscard]] board_pattern *getEffectRange(Effect_List effect) override;
    bool passive() override;
    bool canEvolve() override;
    bool evolvedForm() override;
    bool SpellActivationCheck() override;
};



#endif //ARTORIA_H
