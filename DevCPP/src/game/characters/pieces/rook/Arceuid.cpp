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
    if (CNTGainEffect >= 8){
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
        if (CNTGainEffect >=16){
            if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
            if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
            if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
            if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);
        }
        if (CNTGainEffect >=24){
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
    std::cout << "CNT : " << CNTGainEffect << std::endl;
    if (canEvolve(context)){
        evolved = true;
        CNTGainEffect = NB_TurnWithoutMoving;
    }
    evolvedForm(context);
    return true;
}

bool Arceuid::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    return true;
}

bool Arceuid::canEvolve(void *arg) {
    if (!evolved && hasRoqued) {
        evolved = true;
        CNTGainEffect = NB_TurnWithoutMoving;
        std::cout << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

bool Arceuid::evolvedForm(void *arg) {
    if (evolved && (CNTGainEffect >= 30 || hasJustKilled && CNTGainEffect >= 8)) {
        EffectHandler::applyEffectToTargets(this,EffectInstance{AOE,1,1,-1});

    }
    return true;
}
