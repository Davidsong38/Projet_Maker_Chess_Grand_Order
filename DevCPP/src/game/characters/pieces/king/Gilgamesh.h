//
// Created by Megaport on 22/02/2025.
//

#ifndef GILGAMESH_H
#define GILGAMESH_H

#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Pieces.h"


class Gilgamesh final: public King{
    protected :
        int CNT_NotMove = 0;
        int CNT_EvolvedForm = 0;
public:
    Gilgamesh(const int startX, const int startY, const bool white, const Characters_List hero)
            : King(startX, startY, white, hero)  {
        addAdditionalUIElement(
            gilgameshTexture,
            glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
            this
        );
    }

    [[nodiscard]] vector<glm::ivec2> getEffectRange(Effect_List effect) override;
    bool passive(void* arg) override;
    bool canEvolve(void* arg) override;
    bool evolvedForm(void* arg) override;
    bool SpellActivationCheck(void *arg) override;
    void setPieceGameMode(int piece_game_mode) override;
};

#endif //GILGAMESH_H
