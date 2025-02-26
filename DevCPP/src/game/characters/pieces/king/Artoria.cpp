//
// Created by david on 31/01/2025.
//

#include "Artoria.h"

void Artoria::setPieceGameMode(int piece_game_mode) {

}

vector<glm::ivec2> Artoria::getEffectRange(Effect_List effect) const {
    vector<glm::ivec2> effect_range;
    const glm::ivec2 lastPos = AllMovesDoneBefore.back();
    if (effect == AOE){
        if (lastPos.x == coordX - 1 && lastPos.y == coordY - 1){
            for (int i = 1; i < 8; ++i){
                if (coordX + i < 8 && coordY + i < 8) effect_range.emplace_back(coordX + i, coordY + i);
                if (coordX + i < 8) effect_range.emplace_back(coordX + i, coordY);
                if (coordY + i < 8) effect_range.emplace_back(coordX, coordY + i);
            }
            return effect_range;
        }
        if (lastPos.x == coordX - 1 && lastPos.y == coordY){
            for (int i = 1; i < 8; ++i){
                if (coordX + i < 8 && coordY + i < 8) effect_range.emplace_back(coordX + i, coordY + i);
                if (coordX + i < 8 && coordY - i >= 0) effect_range.emplace_back(coordX + i, coordY - i);
                if (coordX + i < 8) effect_range.emplace_back(coordX + i, coordY);
            }
            return effect_range;
        }
        if (lastPos.x == coordX - 1 && lastPos.y == coordY + 1){
            for (int i = 1; i < 8; ++i){
                if (coordX + i < 8 && coordY - i >= 0) effect_range.emplace_back(coordX + i, coordY - i);
                if (coordX + i < 8) effect_range.emplace_back(coordX + i, coordY);
                if (coordY - i >= 0) effect_range.emplace_back(coordX, coordY - i);
            }
            return effect_range;
        }
        if (lastPos.x == coordX && lastPos.y == coordY - 1){
            for (int i = 1; i < 8; ++i){
                if (coordX + i < 8 && coordY + i < 8) effect_range.emplace_back(coordX + i, coordY + i);
                if (coordX - i >= 0 && coordY + i < 8) effect_range.emplace_back(coordX - i, coordY + i);
                if (coordY + i < 8) effect_range.emplace_back(coordX, coordY + i);
            }
            return effect_range;
        }
        if (lastPos.x == coordX && lastPos.y == coordY + 1){
            for (int i = 1; i < 8; ++i){
                if (coordX + i < 8 && coordY - i >= 0) effect_range.emplace_back(coordX + i, coordY - i);
                if (coordX - i >= 0 && coordY - i >= 0) effect_range.emplace_back(coordX - i, coordY - i);
                if (coordY - i >= 0) effect_range.emplace_back(coordX, coordY - i);
            }
            return effect_range;
        }
        if (lastPos.x == coordX + 1 && lastPos.y == coordY - 1){
            for (int i = 1; i < 8; ++i){
                if (coordX - i >= 0 && coordY + i < 8) effect_range.emplace_back(coordX - i, coordY + i);
                if (coordX - i >= 0) effect_range.emplace_back(coordX - i, coordY);
                if (coordY + i < 8) effect_range.emplace_back(coordX, coordY + i);
            }
            return effect_range;
        }
        if (lastPos.x == coordX + 1 && lastPos.y == coordY){
            for (int i = 1; i < 8; ++i){
                if (coordX - i >= 0 && coordY + i < 8) effect_range.emplace_back(coordX - i, coordY + i);
                if (coordX - i >= 0 && coordY - i >= 0) effect_range.emplace_back(coordX - i, coordY - i);
                if (coordX - i >= 0) effect_range.emplace_back(coordX - i, coordY);
            }
            return effect_range;
        }
        if (lastPos.x == coordX + 1 && lastPos.y == coordY + 1){
            for (int i = 1; i < 8; ++i){
                if (coordX - i >= 0 && coordY - i >= 0) effect_range.emplace_back(coordX - i, coordY - i);
                if (coordX - i >= 0) effect_range.emplace_back(coordX - i, coordY);
                if (coordY - i >= 0) effect_range.emplace_back(coordX, coordY - i);
            }
            return effect_range;
        }
    }
    return effect_range;
}

bool Artoria::SpellActivationCheck(void *arg) {
    evolvedForm(arg);
    if (canEvolve(arg)){
        CNT_GodMove = 3;
        evolved = true;
        default_piece_move = queen_default_moves;
    }
    if (hasJustKilled){
        passive(arg);
    }
    if (CNT_GodMove == 0) {
        evolved = false;
        default_piece_move = king_default_moves;
    }
    return true;
}

bool Artoria::passive(void* arg) {
    return true;
}

bool Artoria::canEvolve(void *arg) {
    if (!evolved && hasJustKilled) {
        return true;
    }
    return false;

}

bool Artoria::evolvedForm(void *arg) {
    if (evolved)
        CNT_GodMove--;
    return true;
}
