//
// Created by david on 02/03/2025.
//

#ifndef KINTOKI_RIDER_H
#define KINTOKI_RIDER_H
#include <Pieces.h>
#include <textures.h>
#include <uiElements.h>


class Kintoki_Rider final : public Knight {
    protected :
        int CNT_SuppMove = 0;
        int suppMoveChance = 50;
public:
    Kintoki_Rider(const int startX, const int startY, const bool white, const Characters_List hero)
            : Knight(startX, startY, white, hero)  {
        defaultEffectsRanges[SUPP_MOVE] = [this](){return this->getEffectRange(SUPP_MOVE);};
        addAdditionalUIElement(
            kintokiTexture,
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



#endif //KINTOKI_RIDER_H
