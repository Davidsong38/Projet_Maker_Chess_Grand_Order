//
// Created by david on 31/01/2025.
//

#ifndef SESSHOIN_KIARA_H
#define SESSHOIN_KIARA_H

#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Pieces.h"


class Sesshoin_Kiara final : public Queen{
protected :
    int CNT_Charm = 0;
public:
    Sesshoin_Kiara(const int startX, const int startY, const bool white, const Characters_List hero)
    : Queen(startX, startY, white, hero)  {
        defaultEffectsRanges[CHANGE_CONTROL] = [this](){return this->getEffectRange(CHANGE_CONTROL);};
        defaultEffectsRanges[CHANGE_CONTROL_ADVANCE] = [this](){return this->getEffectRange(CHANGE_CONTROL_ADVANCE);};
        addAdditionalUIElement(
            kiaraTexture,
            glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
            this
        );
    }

    [[nodiscard]] board_pattern *getEffectRange(Effect_List effect) override;
    bool passive() override;
    bool canEvolve() override;
    bool evolvedForm() override;
    bool SpellActivationCheck() override;
    bool togglePieceGameMode() override;
};



#endif //SESSHOIN_KIARA_H
