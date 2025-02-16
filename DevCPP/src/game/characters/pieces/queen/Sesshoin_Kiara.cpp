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


vector<pair<int, int> > Sesshoin_Kiara::getEffectRange(Effect_List effect) const {

    vector<std::pair<int, int>> effect_range;
    if (evolved && effect == CHANGE_CONTROL_ADVANCE){
        for (int i = 0; i < 7; ++i){
            for (int j = 0; j < 7; ++j){
                effect_range.emplace_back(i, j);
            }
        }
    }

    if (effect == CHANGE_CONTROL) {
        for (int i = 1; i < 3; ++i) {
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
    if (context->piece->getPieceGameMode() != 0){
        if (canEvolve(context)){
            evolvedForm(context);
            return;
        }
        passive(context);
    }


}


void Sesshoin_Kiara::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
        if (EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{CHANGE_CONTROL,3,1,1}))
            CNT_Charm++;

}

bool Sesshoin_Kiara::canEvolve(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (evolved == false && CNT_Charm > 2) {
        //std::cout << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;
}

void Sesshoin_Kiara::evolvedForm(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    evolved = true;
    EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{CHANGE_CONTROL_ADVANCE,3,1,1});
}