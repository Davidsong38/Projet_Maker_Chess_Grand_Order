//
// Created by Megaport on 20/02/2025.
//

#ifndef XU_FU_H
#define XU_FU_H

#include <Pieces.h>
#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"



class Xu_Fu final : public Rook{
    protected :
    int CNT_Shield = 0;
    int shieldChance = 90;
public:
    Xu_Fu(const int startX, const int startY, const bool white, const Characters_List hero)
            : Rook(startX, startY, white, hero)  {
        defaultEffectsRanges[IMMORTALITY] = [this](){return this->getEffectRange(IMMORTALITY);};
        defaultEffectsRanges[SHIELD] = [this](){return this->getEffectRange(SHIELD);};
        defaultEffectsRanges[SUPP_RANGE] = [this](){return this->getEffectRange(SUPP_RANGE);};
        addAdditionalUIElement(
            xufuTexture,
            glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
            this
        );
    }


    [[nodiscard]] board_pattern *getEffectRange(Effect_List effect) override;
    bool passive() override;
    bool canEvolve() override {return evolved == false && CNT_Shield == 2;}
    bool evolvedForm() override;
    bool SpellActivationCheck() override;
    bool togglePieceGameMode() override;

};



#endif //XU_FU_H
