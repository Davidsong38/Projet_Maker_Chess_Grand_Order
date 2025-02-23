//
// Created by Megaport on 22/02/2025.
//

#include "Gilgamesh.h"


void Gilgamesh::setPieceGameMode(int piece_game_mode) {
    if (!evolved)
        pieceGameMode = piece_game_mode;
}

vector<pair<int, int> > Gilgamesh::getMoves() {
    vector<std::pair<int, int>> moves;

    if (!evolved || movesMode == 0){
        if (coordX + 1 < 8 && coordY + 1 < 8) moves.emplace_back(coordX + 1, coordY + 1);
        if (coordX - 1 >= 0 && coordY + 1 < 8) moves.emplace_back(coordX - 1, coordY + 1);
        if (coordX + 1 < 8 && coordY- 1 >= 0) moves.emplace_back(coordX + 1, coordY - 1);
        if (coordX - 1 >= 0 && coordY - 1 >= 0) moves.emplace_back(coordX - 1, coordY - 1);
        if (coordX + 1 < 8) moves.emplace_back(coordX + 1, coordY);
        if (coordX - 1 >= 0) moves.emplace_back(coordX - 1, coordY);
        if (coordY- 1 >= 0) moves.emplace_back(coordX, coordY - 1);
        if (coordY + 1 < 8) moves.emplace_back(coordX, coordY + 1);
    }
    if (movesMode == 1 && evolved){
        for (int i = 1; i < 8; ++i) {
            if (coordX + i < 8) moves.emplace_back(coordX + i, coordY);
            if (coordX - i >= 0) moves.emplace_back(coordX - i, coordY);
            if (coordY- i >= 0) moves.emplace_back(coordX, coordY - i);
            if (coordY + i < 8) moves.emplace_back(coordX, coordY + i);
        }
    }

    if (movesMode == 2 && evolved){
        for (int i = 1; i < 8; ++i) {
            if (coordX + i < 8 && coordY + i < 8) moves.emplace_back(coordX + i, coordY + i);
            if (coordX - i >= 0 && coordY + i < 8) moves.emplace_back(coordX - i, coordY + i);
            if (coordX + i < 8 && coordY- i >= 0) moves.emplace_back(coordX + i, coordY - i);
            if (coordX - i >= 0 && coordY - i >= 0) moves.emplace_back(coordX - i, coordY - i);
        }
    }
    if (movesMode == 3 && evolved){
        if (coordX + 1 < 8 && coordY + 2 < 8) moves.emplace_back(coordX + 1, coordY + 2);
        if (coordX - 1 >= 0 && coordY + 2 < 8) moves.emplace_back(coordX - 1, coordY + 2);
        if (coordX + 1 < 8 && coordY- 2 >= 0) moves.emplace_back(coordX + 1, coordY - 2);
        if (coordX - 1 >= 0 && coordY - 2 >= 0) moves.emplace_back(coordX - 1, coordY - 2);
        if (coordX + 2 < 8 && coordY + 1 < 8) moves.emplace_back(coordX + 2, coordY + 1);
        if (coordX - 2 >= 0 && coordY + 1 < 8) moves.emplace_back(coordX - 2, coordY + 1);
        if (coordX + 2 < 8 && coordY- 1 >= 0) moves.emplace_back(coordX + 2, coordY - 1);
        if (coordX - 2 >= 0 && coordY - 1 >= 0) moves.emplace_back(coordX - 2, coordY - 1);

    }

    if (movesMode == 4 && evolved){
        for (int i = 1; i < 8; ++i) {
            if (coordX + i < 8 && coordY + i < 8) moves.emplace_back(coordX + i, coordY + i);
            if (coordX - i >= 0 && coordY + i < 8) moves.emplace_back(coordX - i, coordY + i);
            if (coordX + i < 8 && coordY- i >= 0) moves.emplace_back(coordX + i, coordY - i);
            if (coordX - i >= 0 && coordY - i >= 0) moves.emplace_back(coordX - i, coordY - i);
            if (coordX + i < 8) moves.emplace_back(coordX + i, coordY);
            if (coordX - i >= 0) moves.emplace_back(coordX - i, coordY);
            if (coordY- i >= 0) moves.emplace_back(coordX, coordY - i);
            if (coordY + i < 8) moves.emplace_back(coordX, coordY + i);
        }
    }

    return moves;
}



vector<pair<int, int> > Gilgamesh::getEffectRange(Effect_List effect) const {

    vector<std::pair<int, int>> effect_range;
    if (effect == MOVE_CHANGING){
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j){
                effect_range.emplace_back(i, j);
            }
        }
    }
    return effect_range;
}

bool Gilgamesh::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (this->getPieceGameMode() != 0 && !evolved){
        passive(context);
        return true;
    }
    if (canEvolve(context)){
            evolved = true;
            CNT_NotMove = 0;
            CNT_EvolvedForm = 4;
    }
    if (evolved && CNT_EvolvedForm == 4){
        if (GameEngine::getInstance()->receivedClick && evolvedForm(context)){
            std::cout << "ZASHHUUU" << std::endl;
            CNT_EvolvedForm--;
            return true;
        }
        return false;
    }
    if (evolved)
        CNT_EvolvedForm--;
    if (CNT_EvolvedForm == 0){
        evolved = false;
        std::cout << "yorokobe" << std::endl;
    }
    return true;
}



bool Gilgamesh::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    if (this != context->piece){
        CNT_NotMove++;
        return true;
    }
    if (GameEngine::getInstance()->getLastState() == SELECT_BLACK_PHASE || GameEngine::getInstance()->getLastState() == SELECT_WHITE_PHASE){
        std::cout <<"MovesMode : " <<movesMode << std::endl;
        movesMode = rand() % 4 + 1;
        std::cout <<"MovesMode !!!! : " <<movesMode << std::endl;
        return true;
    }
    CNT_NotMove = 0;
    return true;
}

bool Gilgamesh::canEvolve(void *arg) {
    if (!evolved && CNT_NotMove >= 8 && (GameEngine::getInstance()->getLastState() == MOVING_BLACK_PHASE
        || GameEngine::getInstance()->getLastState() == MOVING_WHITE_PHASE)) {
        std::cout << CNT_NotMove << std::endl;
        std::cout << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

bool Gilgamesh::evolvedForm(void *arg) {
    if (EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{MOVE_CHANGING,-1,3,1,this}))
        return true;
    return false;
}
