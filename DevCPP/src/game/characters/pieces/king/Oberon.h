//
// Created by david on 27/03/2025.
//

#ifndef OBERON_H
#define OBERON_H



#include <Pieces.h>
#include <RenderEngine.h>
#include <rendering_cfg.h>
#include <textures.h>
#include <uiElements.h>


class Oberon final : public Bishop {
protected :
    int CNT_Success = 0;

public:
    Oberon(const int startX, const int startY, const bool white, const Characters_List hero)
            : Bishop(startX, startY, white, hero) {
        defaultEffectsRanges[KILLING] = [this](){return this->getEffectRange(KILLING);};
        defaultEffectsRanges[GIVING_AOE] = [this](){return this->getEffectRange(GIVING_AOE);};
        defaultEffectsRanges[ALLY_TELEPORT] = [this](){return this->getEffectRange(ALLY_TELEPORT);};
        addAdditionalUIElement(
            oberonTexture,
            glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
            this
        );
    }
    [[nodiscard]] EffectInstance *getEffectInstanceGiven(Effect_List effect) override;
    [[nodiscard]] board_pattern *getEffectRangeGiven(Effect_List effect) override;
    [[nodiscard]] board_pattern *getEffectRange(Effect_List effect) override;
    bool passive() override;
    bool canEvolve() override;
    bool evolvedForm() override;
    bool SpellActivationCheck() override;

};



#endif //OBERON_H
