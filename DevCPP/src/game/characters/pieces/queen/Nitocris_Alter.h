//
// Created by Megaport on 21/02/2025.
//

#ifndef NITOCRIS_ALTER_H
#define NITOCRIS_ALTER_H
#include <Queen.h>
#include <RenderEngine.h>
#include <rendering_cfg.h>
#include <textures.h>
#include <uiElements.h>


class Nitocris_Alter final : public Queen{
    protected :
        int Revive_Charge = 0;
        int CNT_Revive = 0;
        int CNT_4Turn = 0;
public:
    Nitocris_Alter(int startX, int startY, bool white, Characters_List hero,
         Pieces_List pieces_root)
        : Queen(startX, startY, white, hero, pieces_root) {
        addAdditionalUIElement(
            nitocrisTexture,
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



#endif //NITOCRIS_ALTER_H
