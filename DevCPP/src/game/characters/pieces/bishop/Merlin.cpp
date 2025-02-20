//
// Created by Megaport on 19/02/2025.
//

#include "Merlin.h"

#include <Context.h>
#include <GameEngine.h>


void Merlin::setPieceGameMode(int piece_game_mode) {
    //std::cout<< MerlinPowerON<< "tuer moi par pitie" << std::endl;
    //std::cout<< pieceGameMode<< "HHHEEEEEELLLLPPPP" << std::endl;
    if (evolved && MerlinPowerON){
        //std::cout<< MerlinPowerON<< "HHHEEEEEELLLLPPPP" << std::endl;
        pieceGameMode = piece_game_mode;
    } else{
        pieceGameMode = 0;
    }
}


vector<pair<int, int> > Merlin::getEffectRange(Effect_List effect) const {

    vector<std::pair<int, int>> effect_range;

    if (effect == IMMUNITY_EFFECT && !chooseSpell) {
        if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
        if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
        if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
        if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);
    }

    if (effect == IMMUNITY_AOE && !chooseSpell) {
        if (coordX + 1 < 8) effect_range.emplace_back(coordX + 1, coordY);
        if (coordX - 1 >= 0) effect_range.emplace_back(coordX - 1, coordY);
        if (coordY - 1 >= 0) effect_range.emplace_back(coordX, coordY - 1);
        if (coordY + 1 < 8) effect_range.emplace_back(coordX, coordY + 1);
    }

    if (chooseSpell){
        if (effect == IMMUNITY_EFFECT) {
            for (int i = 0; i < 8; ++i){
                for (int j = 0; j < 8; ++j){
                    effect_range.emplace_back(i, j);
                }
            }
        }
        if (effect == IMMUNITY_AOE) {
            for (int i = 0; i < 8; ++i){
                for (int j = 0; j < 8; ++j){
                    effect_range.emplace_back(i, j);
                }
            }
        }
    }
    return effect_range;
}

bool Merlin::SpellActivationCheck(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    //std::cout << "merlin power : " << MerlinPowerON <<std::endl;
    if (evolved && MerlinPowerON){
        if (GameEngine::getInstance()->receivedRightClick){
            //std::cout << "gekooo" << std::endl;
            if (this->getIsWhite()){
                GameEngine::getInstance()->setLastState(GameEngine::getInstance()->getCurrentState());
                GameEngine::getInstance()->setState(SELECT_WHITE_PHASE);
            }
            else{
                GameEngine::getInstance()->setLastState(GameEngine::getInstance()->getCurrentState());
                GameEngine::getInstance()->setState(SELECT_BLACK_PHASE);
            }
            return true;
        }
        if (this->getPieceGameMode() != 0){
            if (GameEngine::getInstance()->receivedClick){
                chooseSpell = true;
                if (evolvedForm(context)){
                    chooseSpell = false;
                    MerlinPowerON = false;
                    return true;
                }
                chooseSpell = false;


            }
            return false;
        }
    }
    if (evolved && !MerlinPowerON && !isOnAMove)
        MerlinPowerON = true;
    passive(context);
    setIsOnAMove(false);
    return true;
}


bool Merlin::passive(void* arg) {
    auto * context = static_cast<context_type *>(arg);
    EffectHandler::applyEffectToTargets(this,EffectInstance{IMMUNITY_EFFECT,-1,1,1,this});
    EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{IMMUNITY_AOE,-1,1,1,this},
        GameEngine::getInstance()->getLastPieceTouchedByEffect()->getCoordX(),GameEngine::getInstance()->getLastPieceTouchedByEffect()->getCoordY());
    return true;
}

bool Merlin::canEvolve(void *arg) {
    //std::cout <<CNT_StunEffect<<std::endl;
    if (evolved == false ) {
        //std::cout <<"Ready to evolve!!!"<<std::endl;
        return true;
    }
    return false;

}

bool Merlin::evolvedForm(void *arg) {
    auto * context = static_cast<context_type *>(arg);
    if (EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{IMMUNITY_EFFECT,-1,1,1,this}) &&
        EffectHandler::applyEffectToSelectionnedTarget(this,EffectInstance{IMMUNITY_AOE,-1,1,1,this})){
        EffectHandler::applyBuffToSelf(this,EffectInstance{ONE_MORE_MOVE,1,1,1});
        return true;
    }
    return false;
}