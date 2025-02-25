//
// Created by david on 31/01/2025.
//

#include "Artoria.h"
#include "Context.h"


//vector<Effect_List> Artoria::getCasterEffects() const {
//    if (evolved==true) {
//        return {STUN,AOE};
//    }
//    return {STUN};
//}
void Artoria::setPieceGameMode(int piece_game_mode) {
    return;
}

vector<glm::ivec2> Artoria::getMoves() {
    vector<glm::ivec2> moves;

    if (!evolved){
        if (coordX + 1 < 8 && coordY + 1 < 8) moves.emplace_back(coordX + 1, coordY + 1);
        if (coordX - 1 >= 0 && coordY + 1 < 8) moves.emplace_back(coordX - 1, coordY + 1);
        if (coordX + 1 < 8 && coordY- 1 >= 0) moves.emplace_back(coordX + 1, coordY - 1);
        if (coordX - 1 >= 0 && coordY - 1 >= 0) moves.emplace_back(coordX - 1, coordY - 1);
        if (coordX + 1 < 8) moves.emplace_back(coordX + 1, coordY);
        if (coordX - 1 >= 0) moves.emplace_back(coordX - 1, coordY);
        if (coordY- 1 >= 0) moves.emplace_back(coordX, coordY - 1);
        if (coordY + 1 < 8) moves.emplace_back(coordX, coordY + 1);
    }

    if (evolved){
        for (int i = 1; i < 8; ++i) {
            if (coordX + i < 8 && coordY + i < 8) moves.emplace_back(coordX + i, coordY + i);
            if (coordX - i >= 0 && coordY + i < 8) moves.emplace_back(coordX - i, coordY + i);
            if (coordX + i < 8 && coordY- i >= 0) moves.emplace_back(coordX + i, coordY - i);
            if (coordX - i >= 0 && coordY - i >= 0) moves.emplace_back(coordX - i, coordY - i);
            if (coordX + i < 8) moves.emplace_back(coordX + i, coordY);
            if (coordX - i >= 0) moves.emplace_back(coordX - i, coordY);
            if (coordY- i >= 0) moves.emplace_back(coordX, coordY - i);
            if (coordY + i < 8) moves.emplace_back(coordX, coordY + i);
        }
    }

    return moves;
}



vector<glm::ivec2> Artoria::getEffectRange(Effect_List effect) const {

    vector<glm::ivec2> effect_range;
    glm::ivec2 lastPos = this->getAllMovesDoneBefore().back();
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
    auto * context = static_cast<context_type *>(arg);
    evolvedForm(context);
    if (canEvolve(context)){
        CNT_GodMove = 3;
        evolved = true;
    }
    if (hasJustKilled){
        passive(context);
    }
    if (CNT_GodMove == 0)
        evolved = false;
    return true;
}


bool Artoria::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    //EffectHandler::applyEffectToTargets(this,EffectInstance{AOE,1,1,-1});


    return true;
}

bool Artoria::canEvolve(void *arg) {
    if (!evolved && hasJustKilled) {
        std::cout << "Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

bool Artoria::evolvedForm(void *arg) {
    if (evolved)
        CNT_GodMove--;
    return true;
}
