//
// Created by david on 31/01/2025.
//

#ifndef SESSHOIN_KIARA_H
#define SESSHOIN_KIARA_H

#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Queen.h"


class Sesshoin_Kiara final : public Queen{
    protected :
    bool selectCharm = false;
    public:
        Sesshoin_Kiara(int startX, int startY, bool white, Characters_List hero,
            Pieces_List pieces_root)
            : Queen(startX, startY, white, hero, pieces_root) {
            addAdditionalUIElement(
                kiaraTexture,
                glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
                this
            );
        }

        [[nodiscard]] vector<pair<int, int>> getMoves() override;
        //[[nodiscard]] vector<Effect_List> getCasterEffects() const override;
        [[nodiscard]] vector<pair<int, int>> getEffectRange(Effect_List effect) const override;
        void passive(void* arg) override;
        bool canEvolve(void* arg) override;
        void evolvedForm(void* arg) override;
        void SpellActivationCheck(void *arg) override;
        void setPieceGameMode();

};



#endif //SESSHOIN_KIARA_H
