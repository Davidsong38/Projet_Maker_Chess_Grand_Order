//
// Created by david on 30/01/2025.
//

#ifndef ARCEUID_H
#define ARCEUID_H

#include <Arceuid.h>
#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Pieces.h"


class Arceuid final: public Rook{
    protected :
        int CNTGainEffect = 0;
    public:
    Arceuid(const int startX, const int startY, const bool white, const Characters_List hero)
        : Rook(startX, startY, white, hero)  {
            defaultEffectsRanges[AOE] = [this](){return this->getEffectRange(AOE);};
            addAdditionalUIElement(
                arcueidTexture,
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



#endif //ARCEUID_H
