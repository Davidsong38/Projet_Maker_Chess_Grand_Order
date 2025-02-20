//
// Created by Megaport on 20/02/2025.
//

#ifndef OKITA_H
#define OKITA_H



#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Knight.h"


class Okita final : public Knight {
    protected :
        int CNT_Charge = 0;
public:
    Okita(int startX, int startY, bool white, Characters_List hero,
        Pieces_List pieces_root)
        : Knight(startX, startY, white, hero, pieces_root) {
        addAdditionalUIElement(
            okitaTexture,
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




#endif //OKITA_H
