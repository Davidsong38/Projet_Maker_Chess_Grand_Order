//
// Created by david on 24/01/2025.
//

#include "Medusa_Saber.h"

#include "Context.h"


//vector<Effect_List> Medusa_Saber::getCasterEffects() const {
//    if (evolved==true) {
//        return {STUN,AOE};
//    }
//    return {STUN};
//}

void Medusa_Saber::setPieceGameMode(int piece_game_mode) {
    return;
}


vector<glm::ivec2> Medusa_Saber::getEffectRange(Effect_List effect) const {

    vector<glm::ivec2> effect_range;

    if (effect == STUN && evolved==false) {
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

bool Medusa_Saber::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (hasJustKilled) {
        if (canEvolve(context) || evolved)
            evolvedForm(context);
        passive(context);
    }
    return true;
}


bool Medusa_Saber::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    auto * effect_instance = new EffectInstance(
        STUN,
        this,
        2,
        2,
        1
    );
    EffectHandler::selectRandomTargetPieces(effect_instance);
    if (EffectHandler::applyToTargets(effect_instance))
        CNT_StunEffect++;
    return true;
}

bool Medusa_Saber::canEvolve(void *arg) {
    //std::cout <<CNT_StunEffect<<std::endl;
    if (evolved == false && CNT_StunEffect>1) {
        //std::cout <<"Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

bool Medusa_Saber::evolvedForm(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    evolved = true;
    auto *  effect_instance = new EffectInstance(
        AOE,
        this,
        1,
        1,
        -1
    );
    EffectHandler::selectRandomTargetPieces(effect_instance);
    EffectHandler::applyToTargets(effect_instance);
    return true;


}
