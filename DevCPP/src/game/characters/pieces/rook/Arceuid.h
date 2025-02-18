//
// Created by david on 30/01/2025.
//

#ifndef ARCEUID_H
#define ARCEUID_H

#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Rook.h"


class Arceuid final: public Rook{
    protected :
        int CNTGainEffect = 0;
    public:
        Arceuid(int startX, int startY, bool white, Characters_List hero,
            Pieces_List pieces_root)
            : Rook(startX, startY, white, hero, pieces_root) {
            addAdditionalUIElement(
                arcueidTexture,
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
        [[nodiscard]] vector<pair<int, int>> getMoves() override;



};



#endif //ARCEUID_H
