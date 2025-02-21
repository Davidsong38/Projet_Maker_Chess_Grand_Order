//
// Created by david on 25/01/2025.
//

#include "EffectHandler.h"

#include <chrono>
#include <constants.hpp>
#include <GameEngine.h>
#include <random>

#include "Chessboard.h"
#include <utility>

#include "Xu_Fu.h"


unordered_map<Effect_List, function<bool()>> EffectHandler::effectBehaviors;

//void EffectHandler::executeEffect(Effect_List effect,Pieces* target_piece) {
//    if (effectBehaviors.find(effect) != effectBehaviors.end()) {
//        effectBehaviors[effect]();
//    }
//}

bool EffectHandler::addEffectBehavior(Effect_List effect, function<bool()> behavior) {
    effectBehaviors[effect] = std::move(behavior);
    bool result = effectBehaviors[effect]();
    return result;
}

bool EffectHandler::configureEffectHandler(Pieces *piece, EffectInstance effect_instance) {
    Effect_List current_effect = effect_instance.getEffect();
    Chessboard* board = Chessboard::getInstance();
    bool success = false;
    //std::cout << "success: " << success << std::endl;
    switch (current_effect){
        case ALLY_TELEPORT :{
            success = addEffectBehavior(ALLY_TELEPORT, [board,piece]() {
                int toX = 0;
                int toY = 0;
                std::cout << "donnes les coord!!!";
                std::cin >> toX >> toY;
                if (toX >= 0 && toX < board->getGrid().size() && toY >= 0 && toY < board->getGrid().size()) {
                    piece->setPosition(toX, toY);
                    return true;
                }
                return false;
            });
            break;
        }
        case AOE : {
            success = addEffectBehavior(AOE, [board,piece]() {
                if (piece->isKing())
                    return false;
                for (const auto& e : piece->getActive_effects()) {
                    if (e.effect == SHIELD || e.effect == IMMORTALITY || e.effect == IMMUNITY_AOE ) {
                        piece->activateEffect(e.effect);
                        return true;
                    }
                }
                //std::cout << "BATARI" <<piece->getCoordX() << ' ' << piece->getCoordY() << "BATARU" << std::endl;
                //board->getGrid()[piece->getCoordX()][piece->getCoordY()] = nullptr;
                //std::cout << board->getGrid()[piece->getCoordX()][piece->getCoordY()] << std::endl;
                piece->setIsAlive(false);
                board->deletePiece(piece);
                return true;
            });
            break;
        }
        case STUN : {
            success = addEffectBehavior(STUN, [piece, effect_instance]() {
                if (piece->isKing())
                    return false;
                for (const auto& e : piece->getActive_effects()) {
                    if (e.effect == IMMUNITY_EFFECT) {
                        piece->activateEffect(e.effect);
                        return true;
                    }
                }
                piece->addEffectStatus(effect_instance);
                return true;
            });
            break;
        }
        case CHANGE_CONTROL : {
            success = addEffectBehavior(CHANGE_CONTROL,[piece,effect_instance]() {
                if (piece->isKing())
                    return false;
                for (const auto& e : piece->getActive_effects()) {
                    if (e.effect == IMMUNITY_EFFECT ) {
                        //std::cout << "AHAHAHAHAHAHAHAHAHAHAHAHAHAHA" << std::endl;
                        if (e.caster_piece != nullptr)
                            std::cout << static_cast<Pieces*>(e.caster_piece)->getName() << std::endl;
                        //std::cout << "AHAHAHAHAHAHAHAHAHAHAHAHAHAHA" << std::endl;
                        piece->activateEffect(e.effect);
                        return true;
                    }
                }
                piece->addEffectStatus(effect_instance);
                //std::cout << effect_instance.effect << std::endl;
                //std::cout << effect_instance.effect_duration << std::endl;
                //std::cout << effect_instance.effect_amount << std::endl;


                piece->setIsWhite(not piece->getIsWhite());
                return true;
            });
            break;
        }
        case CHANGE_CONTROL_ADVANCE : {
            success = addEffectBehavior(CHANGE_CONTROL_ADVANCE,[piece,effect_instance]() {
                if (piece->isKing())
                    return false;
                for (const auto& e : piece->getActive_effects()) {
                    if (e.effect == IMMUNITY_EFFECT ) {
                        piece->activateEffect(e.effect);
                        return true;
                    }
                }
                piece->addEffectStatus(effect_instance);
                piece->setIsWhite(not piece->getIsWhite());
                return true;
            });
            break;
        }
        case ONE_MORE_MOVE : {
            success = addEffectBehavior(ONE_MORE_MOVE,[piece](){
                piece->setIsOnAMove(true);
                if (piece->getIsWhite()){
                    GameEngine::getInstance()->setLastState(GameEngine::getInstance()->getCurrentState());
                    GameEngine::getInstance()->setState(SELECT_WHITE_PHASE);
                } else{
                    GameEngine::getInstance()->setLastState(GameEngine::getInstance()->getCurrentState());
                    GameEngine::getInstance()->setState(SELECT_BLACK_PHASE);
                }
                return true;
            });
            break;
        }
        case IMMUNITY_AOE :{
            success = addEffectBehavior(IMMUNITY_AOE,[piece,effect_instance](){
                if (piece->isKing())
                    return false;
                piece->addEffectStatus(effect_instance);
                return true;
            });
            break;
        }

        case IMMUNITY_EFFECT :{
            success = addEffectBehavior(IMMUNITY_EFFECT,[piece,effect_instance](){
                if (piece->isKing())
                    return false;
                piece->addEffectStatus(effect_instance);
                return true;
            });
            break;
        }
        case IMMORTALITY :{
            success = addEffectBehavior(IMMORTALITY,[piece,effect_instance](){
                if (piece->isKing())
                    return false;
                if (effect_instance.caster_piece != nullptr && static_cast<Pieces*>(effect_instance.caster_piece)->getCharacters() == XU_FU){
                    if (piece->isPawn()){
                        piece->addEffectStatus(effect_instance);
                        return true;
                    }
                    return false;
                }
                piece->addEffectStatus(effect_instance);
                return true;
            });
            break;
        }
        case SHIELD :{
            success = addEffectBehavior(SHIELD,[piece,effect_instance](){
                if (effect_instance.caster_piece != nullptr && static_cast<Pieces*>(effect_instance.caster_piece)->getCharacters() == XU_FU){
                    if (piece->isPawn()){
                        piece->addEffectStatus(effect_instance);
                        return true;
                    }
                    return false;
                }
                piece->addEffectStatus(effect_instance);
                return true;
            });
            break;
        }


    default:
        std::cout << "Effect handler undefined" << std::endl;
    }
    std::cout << "success: " << success << std::endl;
    return success;
}

bool EffectHandler::isBuff(Effect_List effect) {
    return isBuff_List[effect];

}
bool EffectHandler::isTriggerEffect(Effect_List effect) {
    return isTriggerEffect_List[effect];
}

bool EffectHandler::validTargetGettingEffect(Pieces *caster_piece, Pieces * target_piece, EffectInstance effect_instance) {
    if ((target_piece != nullptr && !Chessboard::isAlly(caster_piece,target_piece) && !isBuff(effect_instance.getEffect()))
        ||(target_piece != nullptr && Chessboard::isAlly(caster_piece,target_piece) && isBuff(effect_instance.getEffect()))) {
        return true;
    }
    return false;
}

bool EffectHandler::isEffectTargetInGrid(Pieces * target_piece) {
    int coordX = target_piece->getCoordX();
    int coordY = target_piece->getCoordY();
    if (coordX >= 0 && coordX < Chessboard::getInstance()->getGrid().size() && coordY >= 0 && coordY < Chessboard::getInstance()->getGrid().size()) {
        return true;
    }
    return false;
}


int EffectHandler::applyEffectToTargets(Pieces *caster_piece, EffectInstance effect_instance) {
    vector<pair<int,int>> effect_range = caster_piece->getEffectRange(effect_instance.getEffect());
    unsigned num = chrono::system_clock::now().time_since_epoch().count();
    shuffle (effect_range.begin(), effect_range.end(), default_random_engine(num));
    int NB_targetTouched = 0;
    int CNT_target = 1;
    for (const auto &range: effect_range) {
        //std::cout << "(" << range.first << ", " << range.second << ")" << std::endl;
        if (effect_instance.getNB_Target() == -1 || CNT_target <= effect_instance.getNB_Target()) {
            int targetX = range.first;
            int targetY = range.second;
            Pieces* target_piece =  Chessboard::getInstance()->getGrid()[targetX][targetY];
            if (target_piece != nullptr && target_piece->isKing() && (!isBuff(effect_instance.getEffect())
                || effect_instance.getEffect() == IMMUNITY_AOE || effect_instance.getEffect() == IMMUNITY_EFFECT || effect_instance.getEffect() == IMMORTALITY ))
                return NB_targetTouched;
            if (validTargetGettingEffect(caster_piece,target_piece,effect_instance) && isEffectTargetInGrid(target_piece)) {
                if (configureEffectHandler(target_piece,effect_instance)) {
                    GameEngine::getInstance()->setLastPieceTouchedByEffect(target_piece);
                    NB_targetTouched++;
                    if (effect_instance.getNB_Target() != -1)
                        CNT_target++;
                    //executeEffect(effect_instance.getEffect(), target_piece);
                    std::cout << "Effect " << Effect_List_to_string[effect_instance.getEffect()] << " applied to piece at (" << targetX << ", " << targetY << ")." << std::endl;
                }


            }
        }
    }
    return NB_targetTouched;
}


int EffectHandler::applyEffectToSelectionnedTarget(Pieces *caster_piece, EffectInstance effect_instance){
    vector<pair<int,int>> effect_range = caster_piece->getEffectRange(effect_instance.getEffect());
    int NB_targetTouched = 0;
    for (const auto &range: effect_range) {
        int targetX = GameEngine::getInstance()->getLastClickX();
        int targetY = GameEngine::getInstance()->getLastClickY();
        //std::cout << "oho" << std::endl;
        Pieces* target_piece =  Chessboard::getInstance()->getGrid()[targetX][targetY];
        if (target_piece != nullptr && target_piece->isKing() && (!isBuff(effect_instance.getEffect())
            || effect_instance.getEffect() == IMMUNITY_AOE || effect_instance.getEffect() == IMMUNITY_EFFECT || effect_instance.getEffect() == IMMORTALITY ))
            return NB_targetTouched;
        if (validTargetGettingEffect(caster_piece,target_piece,effect_instance) && isEffectTargetInGrid(target_piece)
            && targetX == range.first && targetY == range.second) {
            //std::cout << "ablacabou" << std::endl;
            if (configureEffectHandler(target_piece,effect_instance)) {
                GameEngine::getInstance()->setLastPieceTouchedByEffect(target_piece);
                NB_targetTouched++;
                //std::cout << "ablacabiiiii" << std::endl;

                //executeEffect(effect_instance.getEffect(), target_piece);
                std::cout << "Effect " << Effect_List_to_string[effect_instance.getEffect()] << " applied to piece at (" << targetX << ", " << targetY << ")." << std::endl;
            }
        }
    }
    std::cout << NB_targetTouched << std::endl;
    return NB_targetTouched;
}

int EffectHandler::applyEffectToSelectionnedTarget(Pieces *caster_piece, EffectInstance effect_instance, int targetX , int targetY){
    vector<pair<int,int>> effect_range = caster_piece->getEffectRange(effect_instance.getEffect());
    int NB_targetTouched = 0;
    for (const auto &range: effect_range) {
        //std::cout << "oho" << std::endl;
        Pieces* target_piece =  Chessboard::getInstance()->getGrid()[targetX][targetY];
        if (target_piece != nullptr && target_piece->isKing() && (!isBuff(effect_instance.getEffect())
            || effect_instance.getEffect() == IMMUNITY_AOE || effect_instance.getEffect() == IMMUNITY_EFFECT || effect_instance.getEffect() == IMMORTALITY ))
            return NB_targetTouched;
        if (validTargetGettingEffect(caster_piece,target_piece,effect_instance) && isEffectTargetInGrid(target_piece)
            && targetX == range.first && targetY == range.second) {

            //std::cout << "ablacabou" << std::endl;
            if (configureEffectHandler(target_piece,effect_instance)) {
                GameEngine::getInstance()->setLastPieceTouchedByEffect(target_piece);
                NB_targetTouched++;
                //std::cout << "ablacabiiiii" << std::endl;

                //executeEffect(effect_instance.getEffect(), target_piece);
                std::cout << "Effect " << Effect_List_to_string[effect_instance.getEffect()] << " applied to piece at (" << targetX << ", " << targetY << ")." << std::endl;
            }
        }
    }
    std::cout << NB_targetTouched << std::endl;
    return NB_targetTouched;
}

bool EffectHandler::applyBuffToSelf(Pieces* caster_piece, EffectInstance effect_instance){
    if (configureEffectHandler(caster_piece,effect_instance)){
        //std::cout << "help me pls" << std::endl;
        GameEngine::getInstance()->setLastPieceTouchedByEffect(caster_piece);
        //executeEffect(effect_instance.getEffect(), caster_piece);
        //std::cout << caster_piece->getActive_effects().size()<<std::endl;
        return true;
    }
    return false;
}
