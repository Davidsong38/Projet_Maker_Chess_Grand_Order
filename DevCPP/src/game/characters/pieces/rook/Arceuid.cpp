//
// Created by david on 30/01/2025.
//

#include "Arceuid.h"
#include <iostream>
#include "Context.h"

//vector<Effect_List> Arceuid::getCasterEffects() const {
//    if (evolved==true) {
//        return {STUN,AOE};
//    }
//    return {STUN};
//}

void Arceuid::setPieceGameMode(int piece_game_mode) {
    return;
}

vector<pair<int, int> > Arceuid::getMoves() {
    vector<std::pair<int, int>> moves;
    for (int i = 1; i < 8; ++i) {
        if (coordX + i < 8) moves.emplace_back(coordX + i, coordY);
        if (coordX - i >= 0) moves.emplace_back(coordX - i, coordY);
        if (coordY- i >= 0) moves.emplace_back(coordX, coordY - i);
        if (coordY + i < 8) moves.emplace_back(coordX, coordY + i);
    }
    if (CNTGainEffect >= 4){
        for (int i = 1; i < 3; ++i){
            if (coordX + i < 8 && coordY + i < 8) moves.emplace_back(coordX + i, coordY + i);
            if (coordX - i >= 0 && coordY + i < 8) moves.emplace_back(coordX - i, coordY + i);
            if (coordX + i < 8 && coordY- i >= 0) moves.emplace_back(coordX + i, coordY - i);
            if (coordX - i >= 0 && coordY - i >= 0) moves.emplace_back(coordX - i, coordY - i);
        }
    }
    return moves;
}



vector<pair<int, int> > Arceuid::getEffectRange(Effect_List effect) const {

    vector<std::pair<int, int>> effect_range;
    if (effect == AOE){
        if (CNTGainEffect >=8){
            if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
            if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
            if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
            if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);
        }
        if (CNTGainEffect >=12){
            if (coordX + 2 < 8) effect_range.emplace_back(coordX + 2, coordY);
            if (coordX - 2 >= 0) effect_range.emplace_back(coordX - 2, coordY);
            if (coordY - 2 >= 0) effect_range.emplace_back(coordX, coordY - 2);
            if (coordY + 2 < 8) effect_range.emplace_back(coordX, coordY + 2);
            if (coordX + 1 < 8 && coordY + 1 < 8) effect_range.emplace_back(coordX + 1, coordY + 1);
            if (coordX - 1 >= 0 && coordY + 1 < 8) effect_range.emplace_back(coordX - 1, coordY + 1);
            if (coordX + 1 < 8 && coordY - 1 >= 0) effect_range.emplace_back(coordX + 1, coordY - 1);
            if (coordX - 1 >= 0 && coordY - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY - 1);
        }
    }
    return effect_range;
}

bool Arceuid::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (context->piece->getHasJustKilled())
        passive(context);
    return true;
}

bool Arceuid::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
        EffectHandler::applyEffectToTargets(context->piece,EffectInstance{STUN,2,2,1});

    return true;
}

bool Arceuid::canEvolve(void *arg) {
    if (evolved == false ) {
        std::cout << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

bool Arceuid::evolvedForm(void *arg) {
    CNTGainEffect = this->getNB_TurnWithoutMoving();
    evolved = true;

    return true;
}
