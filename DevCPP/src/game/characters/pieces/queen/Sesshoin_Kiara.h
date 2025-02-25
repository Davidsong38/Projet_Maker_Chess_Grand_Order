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
            addAdditionalUIElement(
                kiaraTexture,
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



#endif //SESSHOIN_KIARA_H
