//
// Created by david on 31/01/2025.
//

#include "Sesshoin_Kiara.h"
#include "Context.h"


//vector<Effect_List> Sesshoin_Kiara::getCasterEffects() const {
//    if (evolved==true) {
//        return {STUN,AOE};
//    }
//    return {STUN};
//}

void Sesshoin_Kiara::setPieceGameMode() {
    int piece_game_mode = 0;
    std::cout << "Choose piece game mode:" << std::endl;
    std::cin >> piece_game_mode;
    pieceGameMode = piece_game_mode;
}


vector<pair<int, int> > Sesshoin_Kiara::getMoves(){
    vector<std::pair<int, int>> moves;
    int selectionMode = 0;
    std::cout << "Choose to Charm?" << std::endl;
    std::cin >> selectionMode;
    if (getPieceGameMode() == 0) {
        for (int i = 1; i < 8; ++i) {
            if (coordX + i < 8 && coordY + i < 8) moves.emplace_back(coordX + i, coordY + i);
            if (coordX - i >= 0 && coordY + i < 8) moves.emplace_back(coordX - i, coordY + i);
            if (coordX + i < 8 && coordY- i >= 0) moves.emplace_back(coordX + i, coordY - i);
            if (coordX - i >= 0 && coordY - i >= 0) moves.emplace_back(coordX - i, coordY - i);
            if (coordX + i < 8) moves.emplace_back(coordX + i, coordY);
            if (coordX - i >= 0) moves.emplace_back(coordX - i, coordY);
            if (coordY - i >= 0) moves.emplace_back(coordX, coordY - i);
            if (coordY + i < 8) moves.emplace_back(coordX, coordY + i);
        }
    }
    return moves;
}

vector<pair<int, int> > Sesshoin_Kiara::getEffectRange(Effect_List effect) const {

    vector<std::pair<int, int>> effect_range;

    if (effect == CHANGE_CONTROL) {
        for (int i = 1; i < 2; ++i) {
            if (coordX + i < 8) effect_range.emplace_back(coordX + i, coordY);
            if (coordX - i >= 0) effect_range.emplace_back(coordX - i, coordY);
            if (coordY - i >= 0) effect_range.emplace_back(coordX, coordY - i);
            if (coordY + i < 8) effect_range.emplace_back(coordX, coordY + i);
        }
    }
    return effect_range;
}

void Sesshoin_Kiara::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (context->piece->getPieceGameMode() == 1)
        passive(context);
}


void Sesshoin_Kiara::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
        EffectHandler::applyEffectToTargets(this,EffectInstance{CHANGE_CONTROL,2,1,1});

}

bool Sesshoin_Kiara::canEvolve(void *arg) {
    if (evolved == false) {
        std::cout << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

void Sesshoin_Kiara::evolvedForm(void *arg) {
    evolved = true;

}