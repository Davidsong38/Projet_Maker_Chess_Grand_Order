//
// Created by Megaport on 19/02/2025.
//

#ifndef MERLIN_H
#define MERLIN_H
#include <Bishop.h>
#include <RenderEngine.h>
#include <rendering_cfg.h>
#include <textures.h>
#include <uiElements.h>


class Merlin final : public Bishop{
    protected :
        bool chooseSpell = false;
        bool MerlinPowerON = false;
public:
    Merlin(int startX, int startY, bool white, Characters_List hero,
         Pieces_List pieces_root)
        : Bishop(startX, startY, white, hero, pieces_root) {
        addAdditionalUIElement(
            merlinTexture,
            glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
            this
        );
    }

    //[[nodiscard]] vector<Effect_List> getCasterEffects() const override;
    [[nodiscard]] vector<glm::ivec2> getEffectRange(Effect_List effect) const override;
    bool passive(void* arg) override;
    bool canEvolve(void* arg) override;
    bool evolvedForm(void* arg) override;
    bool SpellActivationCheck(void *arg) override;
    void setPieceGameMode(int piece_game_mode) override;



};



#endif //MERLIN_H
