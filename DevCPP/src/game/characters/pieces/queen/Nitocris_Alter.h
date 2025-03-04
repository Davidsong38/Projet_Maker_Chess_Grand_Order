//
// Created by Megaport on 21/02/2025.
//

#ifndef NITOCRIS_ALTER_H
#define NITOCRIS_ALTER_H

#include <Pieces.h>
#include <RenderEngine.h>
#include <rendering_cfg.h>
#include <textures.h>
#include <uiElements.h>


class Nitocris_Alter final : public Queen{
    protected :
        int Revive_Charge = 0;
        int CNT_Revive = 0;
        int CNT_4Turn = 0;
public:
    Nitocris_Alter(const int startX, const int startY, const bool white, const Characters_List hero)
            : Queen(startX, startY, white, hero)  {
        defaultEffectsRanges[SPAWN_PIECES] = [this](){return this->getEffectRange(SPAWN_PIECES);};
        defaultEffectsRanges[KILLING] = [this](){return this->getEffectRange(KILLING);};
        this->default_piece_move = inverted_shinji_moves;
        addAdditionalUIElement(
            nitocrisTexture,
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



#endif //NITOCRIS_ALTER_H
