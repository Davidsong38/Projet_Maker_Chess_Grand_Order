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

bool EffectHandler::configureEffectHandler(int coordX, int coordY,Pieces *piece, EffectInstance effect_instance) {
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
                if (piece->getIsWhite())
                    GameEngine::getInstance()->NB_WhiteDead++;
                if (!piece->getIsWhite())
                    GameEngine::getInstance()->NB_BlackDead++;
                board->addToDeadList(piece);
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
        case SUPP_RANGE :{
            success = addEffectBehavior(SUPP_RANGE,[piece,effect_instance](){
                //std::cout << " allllloooo " << std::endl;
                if (effect_instance.caster_piece != nullptr && static_cast<Pieces*>(effect_instance.caster_piece)->getCharacters() == XU_FU){
                    //std::cout << " allllliiiii " << std::endl;
                    if (piece->isPawn()){
                       // std::cout << " alllllaaa " << std::endl;
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
        case KILLING :{
            success = addEffectBehavior(KILLING,[board,piece,effect_instance](){
                if (piece->isKing())
                    return false;
                if (effect_instance.caster_piece != nullptr && static_cast<Pieces*>(effect_instance.caster_piece)->getCharacters() == NITOCRIS_ALTER){
                    if (piece->isPawn()){
                        for (const auto& e : piece->getActive_effects()) {
                            if (e.effect == IMMUNITY_EFFECT || e.effect == IMMORTALITY) {
                                piece->activateEffect(e.effect);
                                return true;
                            }
                        }
                        if (piece->getIsWhite())
                            GameEngine::getInstance()->NB_WhiteDead++;
                        if (!piece->getIsWhite())
                            GameEngine::getInstance()->NB_BlackDead++;
                        board->addToDeadList(piece);
                        piece->setIsAlive(false);
                        board->deletePiece(piece);
                        return true;
                    }
                    return false;
               }
                for (const auto& e : piece->getActive_effects()) {
                    if (e.effect == IMMUNITY_EFFECT || e.effect == IMMORTALITY) {
                        piece->activateEffect(e.effect);
                        return true;
                    }
                }
                if (piece->getIsWhite())
                    GameEngine::getInstance()->NB_WhiteDead++;
                if (!piece->getIsWhite())
                    GameEngine::getInstance()->NB_BlackDead++;
                board->addToDeadList(piece);
                piece->setIsAlive(false);
                board->deletePiece(piece);
                return true;
            });
            break;
        }
        case SPAWN_PIECES :{
            success = addEffectBehavior(SPAWN_PIECES,[coordX,coordY,board,effect_instance](){
                std::cout << "coordX : " << coordX << " coordY : " << coordY << std::endl;
                long long rd = rand() % board->getDeadList().size();
                for (long long i = 0; i < board->getDeadList().size(); i++) {
                    long long id = (rd + i) % board->getDeadList().size();
                    Pieces* resurrectPiece = board->getDeadList()[id];
                    if (resurrectPiece != nullptr && effect_instance.caster_piece !=nullptr
                        && resurrectPiece->getIsWhite() == static_cast<Pieces*>(effect_instance.caster_piece)->getIsWhite()){
                        std::cout << "coordX : " << coordX << " coordY : " << coordY << std::endl;
                        (*board->getGrid_ptr())[coordX][coordY] = resurrectPiece;
                        resurrectPiece->setIsAlive(true);
                        resurrectPiece->setPosition(coordX, coordY);
                        return true;
                        //board->getDeadList().erase(board->getDeadList().begin()+id);
                    }
                }
                return true;
            });
            break;
        }
        case MOVE_CHANGING :{
            success = addEffectBehavior(MOVE_CHANGING,[piece,effect_instance](){
                if (effect_instance.caster_piece != nullptr && static_cast<Pieces*>(effect_instance.caster_piece)->getCharacters() == GILGAMESH){
                    auto* casterPiece = static_cast<Pieces*>(effect_instance.caster_piece);
                    if (casterPiece->getMovesMode() == 0){
                        piece->setOverrideMoves([piece]()->std::vector<pair<int,int>>{
                            piece->setMovesMode(0);
                            vector<std::pair<int, int>> moves;
                            int coordX = piece->getCoordX();
                            int coordY = piece->getCoordY();
                            if (coordX + 1 < 8 && coordY + 1 < 8) moves.emplace_back(coordX + 1, coordY + 1);
                            if (coordX - 1 >= 0 && coordY + 1 < 8) moves.emplace_back(coordX - 1, coordY + 1);
                            if (coordX + 1 < 8 && coordY- 1 >= 0) moves.emplace_back(coordX + 1, coordY - 1);
                            if (coordX - 1 >= 0 && coordY - 1 >= 0) moves.emplace_back(coordX - 1, coordY - 1);
                            if (coordX + 1 < 8) moves.emplace_back(coordX + 1, coordY);
                            if (coordX - 1 >= 0) moves.emplace_back(coordX - 1, coordY);
                            if (coordY- 1 >= 0) moves.emplace_back(coordX, coordY - 1);
                            if (coordY + 1 < 8) moves.emplace_back(coordX, coordY + 1);
                            return moves;
                        });
                    }
                    if (casterPiece->getMovesMode() == 1){
                        piece->setOverrideMoves([piece]()->std::vector<pair<int,int>>{
                            piece->setMovesMode(1);
                            vector<std::pair<int, int>> moves;
                            int coordX = piece->getCoordX();
                            int coordY = piece->getCoordY();
                            for (int i = 1; i < 8; ++i) {
                                if (coordX + i < 8) moves.emplace_back(coordX + i, coordY);
                                if (coordX - i >= 0) moves.emplace_back(coordX - i, coordY);
                                if (coordY- i >= 0) moves.emplace_back(coordX, coordY - i);
                                if (coordY + i < 8) moves.emplace_back(coordX, coordY + i);
                            }
                            return moves;
                        });
                    }
                    if (casterPiece->getMovesMode() == 2){
                        piece->setOverrideMoves([piece]()->std::vector<pair<int,int>>{
                            piece->setMovesMode(2);
                            vector<std::pair<int, int>> moves;
                            int coordX = piece->getCoordX();
                            int coordY = piece->getCoordY();
                            for (int i = 1; i < 8; ++i) {
                                if (coordX + i < 8 && coordY + i < 8) moves.emplace_back(coordX + i, coordY + i);
                                if (coordX - i >= 0 && coordY + i < 8) moves.emplace_back(coordX - i, coordY + i);
                                if (coordX + i < 8 && coordY- i >= 0) moves.emplace_back(coordX + i, coordY - i);
                                if (coordX - i >= 0 && coordY - i >= 0) moves.emplace_back(coordX - i, coordY - i);
                            }
                            return moves;
                        });
                    }
                    if (casterPiece->getMovesMode() == 3){
                        piece->setOverrideMoves([piece]()->std::vector<pair<int,int>>{
                            piece->setMovesMode(3);
                            vector<std::pair<int, int>> moves;
                            int coordX = piece->getCoordX();
                            int coordY = piece->getCoordY();
                            if (coordX + 1 < 8 && coordY + 2 < 8) moves.emplace_back(coordX + 1, coordY + 2);
                            if (coordX - 1 >= 0 && coordY + 2 < 8) moves.emplace_back(coordX - 1, coordY + 2);
                            if (coordX + 1 < 8 && coordY- 2 >= 0) moves.emplace_back(coordX + 1, coordY - 2);
                            if (coordX - 1 >= 0 && coordY - 2 >= 0) moves.emplace_back(coordX - 1, coordY - 2);
                            if (coordX + 2 < 8 && coordY + 1 < 8) moves.emplace_back(coordX + 2, coordY + 1);
                            if (coordX - 2 >= 0 && coordY + 1 < 8) moves.emplace_back(coordX - 2, coordY + 1);
                            if (coordX + 2 < 8 && coordY- 1 >= 0) moves.emplace_back(coordX + 2, coordY - 1);
                            if (coordX - 2 >= 0 && coordY - 1 >= 0) moves.emplace_back(coordX - 2, coordY - 1);
                            return moves;
                        });
                    }
                    if (casterPiece->getMovesMode() == 4){
                        piece->setOverrideMoves([piece]()->std::vector<pair<int,int>>{
                            piece->setMovesMode(4);
                            vector<std::pair<int, int>> moves;
                            int coordX = piece->getCoordX();
                            int coordY = piece->getCoordY();
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
                            return moves;
                        });
                    }
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
    if (target_piece != nullptr && target_piece->isKing() && (!isBuff(effect_instance.getEffect())
        || effect_instance.getEffect() == IMMUNITY_AOE || effect_instance.getEffect() == IMMUNITY_EFFECT || effect_instance.getEffect() == IMMORTALITY ))
        return false;
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

bool EffectHandler::isEffectTargetInGrid(int coordX, int coordY){
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
            if (validTargetGettingEffect(caster_piece,target_piece,effect_instance) && isEffectTargetInGrid(target_piece)) {
                if (configureEffectHandler(targetX,targetY, target_piece,effect_instance)) {
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
        if (validTargetGettingEffect(caster_piece,target_piece,effect_instance) && isEffectTargetInGrid(target_piece)
            && targetX == range.first && targetY == range.second) {
            //std::cout << "ablacabou" << std::endl;
            if (configureEffectHandler(targetX,targetY,target_piece,effect_instance)) {
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
        if (validTargetGettingEffect(caster_piece,target_piece,effect_instance) && isEffectTargetInGrid(target_piece)
            && targetX == range.first && targetY == range.second) {

            //std::cout << "ablacabou" << std::endl;
            if (configureEffectHandler(targetX,targetY,target_piece,effect_instance)) {
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
    if (configureEffectHandler(caster_piece->getCoordX(),caster_piece->getCoordY(),caster_piece,effect_instance)){
        //std::cout << "help me pls" << std::endl;
        GameEngine::getInstance()->setLastPieceTouchedByEffect(caster_piece);
        //executeEffect(effect_instance.getEffect(), caster_piece);
        //std::cout << caster_piece->getActive_effects().size()<<std::endl;
        return true;
    }
    return false;
}

bool EffectHandler::applyToEmptyCell(Pieces* caster_piece, EffectInstance effect_instance){
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
            if (target_piece==nullptr && isEffectTargetInGrid(targetX,targetY)) {
                if (configureEffectHandler(targetX,targetY,target_piece,effect_instance)) {
                    //GameEngine::getInstance()->setLastPieceTouchedByEffect(target_piece);
                    NB_targetTouched++;
                    if (effect_instance.getNB_Target() != -1)
                        CNT_target++;
                    //executeEffect(effect_instance.getEffect(), target_piece);
                    std::cout << "Effect " << Effect_List_to_string[effect_instance.getEffect()] << " applied to cell at (" << targetX << ", " << targetY << ")." << std::endl;
                }


            }
        }
    }
    return NB_targetTouched;
}
