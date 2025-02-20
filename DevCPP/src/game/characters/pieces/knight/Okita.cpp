//
// Created by Megaport on 20/02/2025.
//

#include "Okita.h"

#include <Context.h>

void Okita::setPieceGameMode(int piece_game_mode) {
    return;
}



vector<pair<int, int> > Okita::getEffectRange(Effect_List effect) const {

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
bool Okita::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);

    passive(context);
    if (canEvolve(context))
        evolved = true;
    if (evolved && CNT_Charge != 0){
        CNT_Charge--;
        evolvedForm(context);
        return true;
    }
    if (CNT_Charge == 0)
        setIsOnAMove(false);

    return true;
}


bool Okita::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    if (!evolved && hasJustKilled && !isOnAMove && !context->target_piece->isPawn()){
        CNT_Charge++;
    }
    return true;
}

bool Okita::canEvolve(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (hasJustKilled && !evolved &&(context->target_piece->isQueen() || context->target_piece->isRook())) {
        std::cout << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

bool Okita::evolvedForm(void *arg) {
    EffectHandler::applyBuffToSelf(this,EffectInstance{ONE_MORE_MOVE,1,1,1});
    return true;
}