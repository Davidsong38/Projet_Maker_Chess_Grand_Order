//
// Created by david on 03/03/2025.
//

#ifndef TAMAMO_H
#define TAMAMO_H



#include <Pieces.h>
#include <RenderEngine.h>
#include <rendering_cfg.h>
#include <textures.h>
#include <uiElements.h>


class Tamamo_No_Mae final : public Bishop {
    protected :
        bool chooseSpell{false}, passiveUsed{false};
        bool MerlinPowerON{false};
public:
    Tamamo_No_Mae(const int startX, const int startY, const bool white, const Characters_List hero)
            : Bishop(startX, startY, white, hero) {
        defaultEffectsRanges[ALTERNATE_RANGE] = [this](){return this->getEffectRange(ALTERNATE_RANGE);};
        defaultEffectsRanges[GIVING_AOE] = [this](){return this->getEffectRange(GIVING_AOE);};
        addAdditionalUIElement(
            tanamoTexture,
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




#endif //TAMAMO_H
