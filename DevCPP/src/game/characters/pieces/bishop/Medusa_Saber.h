//
// Created by david on 24/01/2025.
//

#ifndef MEDUSA_SABER_H
#define MEDUSA_SABER_H

#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Pieces.h"


class Medusa_Saber final : public Bishop{
    protected :
        int CNT_StunEffect = 0;
    public:
        Medusa_Saber(const int startX, const int startY, const bool white, const Characters_List hero)
            : Bishop(startX, startY, white, hero) {
            defaultEffectsRanges[AOE] = [this](){return this->getEffectRange(AOE);};
            defaultEffectsRanges[STUN] = [this](){return this->getEffectRange(STUN);};
            addAdditionalUIElement(
                medusaTexture,
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



#endif //MEDUSA_SABER_H
