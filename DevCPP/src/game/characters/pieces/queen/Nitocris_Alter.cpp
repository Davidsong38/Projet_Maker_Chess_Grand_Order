//
// Created by Megaport on 21/02/2025.
//

#include "Nitocris_Alter.h"

#include <Context.h>
#include <GameEngine.h>


void Nitocris_Alter::setPieceGameMode(int piece_game_mode) {
    return;
}


vector<pair<int, int> > Nitocris_Alter::getEffectRange(Effect_List effect) const {

    vector<std::pair<int, int>> effect_range;

    if (effect == SPAWN_PIECES){
        for (int i = 1; i < 3; ++i) {
            if (coordX + i < 8) effect_range.emplace_back(coordX + i, coordY);
            if (coordX - i >= 0) effect_range.emplace_back(coordX - i, coordY);
            if (coordY- i >= 0) effect_range.emplace_back(coordX, coordY - i);
            if (coordY + i < 8) effect_range.emplace_back(coordX, coordY + i);
        }
        if (coordX + 1 < 8 && coordY + 1 < 8) effect_range.emplace_back(coordX + 1, coordY + 1);
        if (coordX - 1 >= 0 && coordY + 1 < 8) effect_range.emplace_back(coordX - 1, coordY + 1);
        if (coordX + 1 < 8 && coordY - 1 >= 0) effect_range.emplace_back(coordX + 1, coordY - 1);
        if (coordX - 1 >= 0 && coordY - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY - 1);
    }
    if (effect == KILLING) {
        for (int i = 0; i < 8; ++i){
            for (int j = 0; j < 8; ++j){
                effect_range.emplace_back(i, j);
            }
        }

    }

    return effect_range;
}

bool Nitocris_Alter::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (evolved && hasJustKilled) {
        evolvedForm(context);
    }
    return true;
}


bool Nitocris_Alter::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);

    if (isWhite){
        CNT_4Turn += GameEngine::getInstance()->NB_WhiteDead - GameEngine::getInstance()->NB_WhiteDeadLastPhase;
    }
    else{
        CNT_4Turn += GameEngine::getInstance()->NB_BlackDead - GameEngine::getInstance()->NB_BlackDeadLastPhase;
    }
    if (CNT_4Turn >= 4){
        Revive_Charge++;
        CNT_4Turn -= 4 ;
    }
    if (Revive_Charge != 0 && EffectHandler::applyToEmptyCell(this,EffectInstance{SPAWN_PIECES,1,1,1,this})){
        CNT_Revive++;
        Revive_Charge--;
    }
    return true;
}

bool Nitocris_Alter::canEvolve(void *arg) {
    //std::cout <<CNT_StunEffect<<std::endl;
    if (evolved == false && CNT_Revive>1) {
        evolved = true;
        //std::cout <<"Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

bool Nitocris_Alter::evolvedForm(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    EffectHandler::applyToEmptyCell(this,EffectInstance{SPAWN_PIECES,1,1,1,this});
    EffectHandler::applyEffectToTargets(this,EffectInstance{KILLING,1,1,1,this});
    return true;


}