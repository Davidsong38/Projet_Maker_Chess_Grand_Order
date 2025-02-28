//
// Created by Megaport on 22/02/2025.
//

#include "Gilgamesh.h"

#include "GameEngine.h"


vector<glm::ivec2> Gilgamesh::getEffectRange(const Effect_List effect) {
    vector<glm::ivec2> effect_range;
    if (effect == MOVE_CHANGING)
        return square_pattern->get_positions(glm::ivec2(coordX, coordY));
    return effect_range;
}

bool Gilgamesh::SpellActivationCheck() {
    return true;
    // const auto* context = GameEngine::getInstance()->getCurrentPhaseContext();
    // if (this->getPieceGameMode() != 0 && !evolved){
    //     passive();
    //     return true;
    // }
    // if (canEvolve()){
    //         evolved = true;
    //         CNT_NotMove = 0;
    //         CNT_EvolvedForm = 4;
    // }
    // if (evolved && CNT_EvolvedForm == 4){
    //     if (GameEngine::getInstance()->receivedClick && evolvedForm()) {
    //         CNT_EvolvedForm--;
    //         return true;
    //     }
    //     return false;
    // }
    // if (evolved)
    //     CNT_EvolvedForm--;
    // if (CNT_EvolvedForm == 0){
    //     evolved = false;
    // }
    // return true;
}

bool Gilgamesh::canEvolve() {
    // if (!evolved && CNT_NotMove >= 8 && (GameEngine::getInstance()->getLastState() == MOVING_BLACK_PHASE
    //     || GameEngine::getInstance()->getLastState() == MOVING_WHITE_PHASE)) {
    //     std::cout << CNT_NotMove << std::endl;
    //     return true;
    // }
    return false;
}

bool Gilgamesh::evolvedForm() {
    //if (EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{MOVE_CHANGING,-1,3,1,this}))
    //    return true;
    return false;
}
