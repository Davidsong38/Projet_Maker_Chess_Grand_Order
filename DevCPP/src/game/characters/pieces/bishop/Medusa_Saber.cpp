//
// Created by david on 24/01/2025.
//

#include "Medusa_Saber.h"

#include "Context.h"


vector<Effect_List> Medusa_Saber::getCasterEffects() const {
    if (evolved==true) {
        return {STUN,AOE};
    }
    return {STUN};
}



vector<pair<int, int> > Medusa_Saber::getEffectRange(Effect_List effect) const {

    vector<std::pair<int, int>> effect_range;

    if (effect == STUN) {
        if (coordX + 1 < 8 && coordY + 1 < 8) effect_range.emplace_back(coordX + 1, coordY + 1);
        if (coordX - 1 >= 0 && coordY + 1 < 8) effect_range.emplace_back(coordX - 1, coordY + 1);
        if (coordX + 1 < 8 && coordY - 1 >= 0) effect_range.emplace_back(coordX + 1, coordY - 1);
        if (coordX - 1 >= 0 && coordY - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY - 1);
    }
    if (evolved==true) {
        if (effect == STUN) {
            if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
            if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
            if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
            if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);

        }
        if (effect == AOE) {
            if (coordX + 1 < 8 && coordY + 1 < 8) effect_range.emplace_back(coordX + 1, coordY + 1);
            if (coordX - 1 >= 0 && coordY + 1 < 8) effect_range.emplace_back(coordX - 1, coordY + 1);
            if (coordX + 1 < 8 && coordY - 1 >= 0) effect_range.emplace_back(coordX + 1, coordY - 1);
            if (coordX - 1 >= 0 && coordY - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY - 1);
        }
    }
    return effect_range;
}


void Medusa_Saber::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    if (context->chessboard->KillCheck(context->piece,context->target_piece)) {
        EffectHandler::applyEffectToTargets(context->piece,EffectInstance{STUN,2,2},*context->chessboard);
        CNT_StunEffect++;


    }
}

bool Medusa_Saber::canEvolve(void *arg) {
    if (evolved == false && CNT_StunEffect>1) {
        std::cout << CNT_StunEffect << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

void Medusa_Saber::evolvedForm(void *arg) {
    evolved = true;

}
