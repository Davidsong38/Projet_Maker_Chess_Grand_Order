//
// Created by david on 31/01/2025.
//

#include "Nemo_Marine.h"
#include "Context.h"


//vector<Effect_List> Nemo_Marine::getCasterEffects() const {
//    if (evolved==true) {
//        return {STUN,AOE};
//    }
//    return {STUN};
//}
void Nemo_Marine::setPieceGameMode(int piece_game_mode) {
    return;
}


vector<pair<int, int> > Nemo_Marine::getEffectRange(Effect_List effect) const {

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

bool Nemo_Marine::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (context->piece->getHasJustKilled())
        passive(context);
    return true;
}


bool Nemo_Marine::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    EffectHandler::applyEffectToTargets(context->piece,EffectInstance{STUN,2,2,1});
    CNT_StunEffect++;

    return true;
}

bool Nemo_Marine::canEvolve(void *arg) {
    if (evolved == false && CNT_StunEffect>1) {
        std::cout << CNT_StunEffect << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

bool Nemo_Marine::evolvedForm(void *arg) {
    evolved = true;

    return true;
}
