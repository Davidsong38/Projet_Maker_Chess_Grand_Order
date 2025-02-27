//
// Created by Megaport on 22/02/2025.
//

#include "Gilgamesh.h"

#include "GameEngine.h"

void Gilgamesh::setPieceGameMode(int piece_game_mode) {
    if (!evolved)
        pieceGameMode = piece_game_mode;
}

vector<glm::ivec2> Gilgamesh::getEffectRange(const Effect_List effect) {
    vector<glm::ivec2> effect_range;
    if (effect == MOVE_CHANGING)
        return square_pattern->get_positions(glm::ivec2(coordX, coordY));
    return effect_range;
}

bool Gilgamesh::SpellActivationCheck(void *arg) {
    return true;
    auto * context = static_cast<phase_context_type *>(arg);
    if (this->getPieceGameMode() != 0 && !evolved){
        passive(arg);
        return true;
    }
    if (canEvolve(context)){
            evolved = true;
            CNT_NotMove = 0;
            CNT_EvolvedForm = 4;
    }
    if (evolved && CNT_EvolvedForm == 4){
        if (GameEngine::getInstance()->receivedClick && evolvedForm(context)) {
            CNT_EvolvedForm--;
            return true;
        }
        return false;
    }
    if (evolved)
        CNT_EvolvedForm--;
    if (CNT_EvolvedForm == 0){
        evolved = false;
    }
    return true;
}

bool Gilgamesh::passive(void* arg) {
    auto * context = static_cast<phase_context_type *>(arg);
    if (this != context->firstSelectedPiece){
        CNT_NotMove++;
        return true;
    }
    if (GameEngine::getInstance()->getLastState() == SELECT_BLACK_PHASE || GameEngine::getInstance()->getLastState() == SELECT_WHITE_PHASE) {
        return true;
    }
    CNT_NotMove = 0;
    return true;
}

bool Gilgamesh::canEvolve(void *arg) {
    if (!evolved && CNT_NotMove >= 8 && (GameEngine::getInstance()->getLastState() == MOVING_BLACK_PHASE
        || GameEngine::getInstance()->getLastState() == MOVING_WHITE_PHASE)) {
        std::cout << CNT_NotMove << std::endl;
        return true;
    }
    return false;
}

bool Gilgamesh::evolvedForm(void *arg) {
    //if (EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{MOVE_CHANGING,-1,3,1,this}))
    //    return true;
    return false;
}
