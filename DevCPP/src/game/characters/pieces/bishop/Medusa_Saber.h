//
// Created by david on 24/01/2025.
//

#ifndef MEDUSA_SABER_H
#define MEDUSA_SABER_H

#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Bishop.h"


class Medusa_Saber final : public Bishop{
    protected :
        int CNT_StunEffect = 0;
    public:
        Medusa_Saber(int startX, int startY, bool white, Characters_List hero,
             Pieces_List pieces_root)
            : Bishop(startX, startY, white, hero, pieces_root) {
            addAdditionalUIElement(
                medusaTexture,
                glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
                this
            );
        }

        //[[nodiscard]] vector<Effect_List> getCasterEffects() const override;
        [[nodiscard]] vector<pair<int, int>> getEffectRange(Effect_List effect) const override;
        bool passive(void* arg) override;
        bool canEvolve(void* arg) override;
        bool evolvedForm(void* arg) override;
        bool SpellActivationCheck(void *arg) override;
        void setPieceGameMode(int piece_game_mode) override;



};



#endif //MEDUSA_SABER_H
