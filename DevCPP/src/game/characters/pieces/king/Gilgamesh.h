//
// Created by Megaport on 22/02/2025.
//

#ifndef GILGAMESH_H
#define GILGAMESH_H



#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "King.h"


class Gilgamesh final: public King{
    protected :
        int CNT_NotMove = 0;
        int CNT_EvolvedForm = 0;
public:
    Gilgamesh(int startX, int startY, bool white, Characters_List hero,
        Pieces_List pieces_root)
        : King(startX, startY, white, hero, pieces_root) {
        addAdditionalUIElement(
            gilgameshTexture,
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

#endif //GILGAMESH_H
