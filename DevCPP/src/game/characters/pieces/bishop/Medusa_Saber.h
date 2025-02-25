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
            addAdditionalUIElement(
                medusaTexture,
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



#endif //MEDUSA_SABER_H
