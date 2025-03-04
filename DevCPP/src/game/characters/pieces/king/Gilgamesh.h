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
        int CNT_EvolvedForm = 0;
        int evolvedFormMove = 0;
public:
    Gilgamesh(const int startX, const int startY, const bool white, const Characters_List hero)
            : King(startX, startY, white, hero)  {
        defaultEffectsRanges[MOVE_CHANGING] = [this](){return this->getEffectRange(MOVE_CHANGING);};
        addAdditionalUIElement(
            gilgameshTexture,
            glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
            this
        );
    }

    [[nodiscard]] board_pattern *getEffectRange(Effect_List effect) override;
    bool passive() override;
    bool canEvolve() override;
    bool evolvedForm() override;
    bool SpellActivationCheck() override;
    bool togglePieceGameMode() override;
};

#endif //GILGAMESH_H
