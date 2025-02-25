//
// Created by david on 31/01/2025.
//

#ifndef NEMO_MARINE_H
#define NEMO_MARINE_H

#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Pieces.h"


class Nemo_Marine final: public Pawn  {
    protected :
        int CNT_StunEffect = 0;
    public:
    Nemo_Marine(const int startX, const int startY, const bool white, const Characters_List hero)
        : Pawn(startX, startY, white, hero)  {
            addAdditionalUIElement(
                nemoMarxTexture,
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

#endif //NEMO_MARINE_H
