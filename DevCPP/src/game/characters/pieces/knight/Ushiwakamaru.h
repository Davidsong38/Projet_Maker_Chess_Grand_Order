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
    protected :
        bool hasCharged = false;
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

        [[nodiscard]] vector<glm::ivec2> getEffectRange(Effect_List effect) const override;
        bool passive(void* arg) override;
        bool canEvolve(void* arg) override;
        bool evolvedForm(void* arg) override;
        bool SpellActivationCheck(void *arg) override;
        void setPieceGameMode(int piece_game_mode) override;
};



#endif //USHIWAKAMARU_H
