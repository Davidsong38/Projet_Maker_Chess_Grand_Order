//
// Created by Megaport on 20/02/2025.
//

#ifndef OKITA_H
#define OKITA_H



#include <textures.h>
#include <uiElements.h>
#include "RenderEngine.h"
#include "rendering_cfg.h"

#include "Pieces.h"


class Okita final : public Knight {
    protected :
        int CNT_Charge = 0;
public:
    Okita(const int startX, const int startY, const bool white, const Characters_List hero)
            : Knight(startX, startY, white, hero)  {
        addAdditionalUIElement(
            okitaTexture,
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




#endif //OKITA_H
