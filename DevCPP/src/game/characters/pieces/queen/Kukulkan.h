//
// Created by david on 03/03/2025.
//

#ifndef KUKULKAN_H
#define KUKULKAN_H
#include <Pieces.h>
#include <textures.h>
#include <uiElements.h>


class Kukulkan final : public Queen {
protected:
    bool enableCNT = true;
    int CNT_Figure = 0;
    int CNT_Kill = 0;
    int CNT_Overcharge = 0;
    int OMM_Chance = 20;
public:
    Kukulkan(const int startX, const int startY, const bool white, const Characters_List hero)
    : Queen(startX, startY, white, hero)  {
        addAdditionalUIElement(
            kukulcanTexture,
            glm::vec2(PIECE_SIZE * RenderEngine::getWindowInverseAspectRatio(), PIECE_SIZE),
            this
        );
    }
    bool passive() override;
    bool canEvolve() override;
    bool evolvedForm() override;
    bool SpellActivationCheck() override;
};



#endif //KUKULKAN_H
