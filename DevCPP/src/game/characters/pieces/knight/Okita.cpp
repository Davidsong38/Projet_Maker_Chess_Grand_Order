//
// Created by Megaport on 20/02/2025.
//

#include "Okita.h"

#include <phase_context.h>

void Okita::setPieceGameMode(int piece_game_mode) {

}

vector<glm::ivec2> Okita::getEffectRange(Effect_List effect) const {
    vector<glm::ivec2> effect_range;
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
    passive(arg);
    if (canEvolve(arg))
        evolved = true;
    if (evolved && CNT_Charge != 0){
        CNT_Charge--;
        evolvedForm(arg);
        return true;
    }
    if (CNT_Charge == 0)
        setIsOnAMove(false);
    return true;
}

bool Okita::passive(void* arg) {
    auto * context = static_cast<phase_context_type *>(arg);
    if (!evolved && hasJustKilled && !isOnAMove && !context->mainTargetPiece->isPawn()){
        CNT_Charge++;
    }
    return true;
}

bool Okita::canEvolve(void *arg) {
    auto * context = static_cast<phase_context_type *>(arg);
    if (hasJustKilled && !evolved &&(context->mainTargetPiece->isQueen() || context->mainTargetPiece->isRook())) {
        return true;
    }
    return false;
}

bool Okita::evolvedForm(void *arg) {
    // EffectHandler::applyBuffToSelf(this,EffectInstance{ONE_MORE_MOVE,1,1,1});
    return true;
}