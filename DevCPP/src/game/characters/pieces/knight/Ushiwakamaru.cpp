//
// Created by david on 31/01/2025.
//

#include "Ushiwakamaru.h"
#include "Context.h"


void Ushiwakamaru::setPieceGameMode(int piece_game_mode) {
    return;
}


vector<pair<int, int> > Ushiwakamaru::getMoves() {
    vector<std::pair<int, int>> moves;
    if (coordX + 1 < 8 && coordY + 2 < 8) moves.emplace_back(coordX + 1, coordY + 2);
    if (coordX - 1 >= 0 && coordY + 2 < 8) moves.emplace_back(coordX - 1, coordY + 2);
    if (coordX + 1 < 8 && coordY- 2 >= 0) moves.emplace_back(coordX + 1, coordY - 2);
    if (coordX - 1 >= 0 && coordY - 2 >= 0) moves.emplace_back(coordX - 1, coordY - 2);
    if (coordX + 2 < 8 && coordY + 1 < 8) moves.emplace_back(coordX + 2, coordY + 1);
    if (coordX - 2 >= 0 && coordY + 1 < 8) moves.emplace_back(coordX - 2, coordY + 1);
    if (coordX + 2 < 8 && coordY- 1 >= 0) moves.emplace_back(coordX + 2, coordY - 1);
    if (coordX - 2 >= 0 && coordY - 1 >= 0) moves.emplace_back(coordX - 2, coordY - 1);
    if (coordX + 1 < 8) moves.emplace_back(coordX + 1, coordY);
    if (coordX - 1 >= 0) moves.emplace_back(coordX - 1, coordY);
    if (coordY- 1 >= 0) moves.emplace_back(coordX, coordY - 1);
    if (coordY + 1 < 8) moves.emplace_back(coordX, coordY + 1);
    return moves;

}



vector<pair<int, int> > Ushiwakamaru::getEffectRange(Effect_List effect) const {

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
bool Ushiwakamaru::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (canEvolve(context))
        evolved = true;
    if (evolved && hasCharged){
        passive(context);
        evolvedForm(context);
        return true;
    }
    if (!passive(context))
        this->setIsOnAMove(false);

    return true;
}


bool Ushiwakamaru::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    if (evolved && hasJustKilled && !isOnAMove){
        if (!hasCharged){
            hasCharged = true;
            return true;
        }
    }
    hasCharged = false;
    return false;
}

bool Ushiwakamaru::canEvolve(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (hasJustKilled && !context->target_piece->isPawn() && !context->target_piece->isRook() && !evolved) {
        std::cout << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

bool Ushiwakamaru::evolvedForm(void *arg) {
    EffectHandler::applyBuffToSelf(this,EffectInstance{ONE_MORE_MOVE,1,1,1});
    return true;
}
