//
// Created by david on 25/01/2025.
//

#include "EffectHandler.h"

#include <chrono>
#include <GameEngine.h>
#include <random>

#include "Chessboard.h"
#include <utility>


unordered_map<Effect_List, function<void()>> EffectHandler::effectBehaviors;

void EffectHandler::executeEffect(Effect_List effect,Pieces* target_piece) {
    if (effectBehaviors.find(effect) != effectBehaviors.end()) {
        effectBehaviors[effect]();
        target_piece->activateEffect(effect);
    }
}

bool EffectHandler::addEffectBehavior(Effect_List effect, function<bool()> behavior) {
    effectBehaviors[effect] = std::move(behavior);
    return true;
}

bool EffectHandler::configureEffectHandler(Pieces *piece, EffectInstance effect_instance) {
    Effect_List current_effect = effect_instance.getEffect();
    Chessboard* board = Chessboard::getInstance();
    bool success = false;
    if (current_effect == ALLY_TELEPORT) {
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
    }
    if (current_effect == AOE) {
        success = addEffectBehavior(AOE, [board,piece]() {
            for (const auto& e : piece->getActive_effects()) {
                if (e.effect == SHIELD || e.effect == IMMORTALITY || e.effect == IMMUNITY_AOE ) {
                    piece->activateEffect(e.effect);
                    return false;
                }
            }
            //std::cout << "BATARI" <<piece->getCoordX() << ' ' << piece->getCoordY() << "BATARU" << std::endl;
            //board->getGrid()[piece->getCoordX()][piece->getCoordY()] = nullptr;
            //std::cout << board->getGrid()[piece->getCoordX()][piece->getCoordY()] << std::endl;
            piece->setIsAlive(false);
            board->deletePiece(piece);
            return true;
        });
    }
    if (current_effect == STUN) {
        success = addEffectBehavior(STUN, [piece, effect_instance]() {
            for (const auto& e : piece->getActive_effects()) {
                if (e.effect == IMMUNITY_AOE ) {
                    piece->activateEffect(e.effect);
                    return false;
                }
            }
            piece->addEffectStatus(effect_instance);
            return true;
        });
    }
    if (current_effect == CHANGE_CONTROL) {
        success = addEffectBehavior(CHANGE_CONTROL,[piece,effect_instance]() {
            for (const auto& e : piece->getActive_effects()) {
                if (e.effect == IMMUNITY_AOE ) {
                    piece->activateEffect(e.effect);
                    return false;
                }
            }
            piece->addEffectStatus(effect_instance);
            piece->setIsWhite(not piece->getIsWhite());
            return true;
        });
    }

    if (current_effect == CHANGE_CONTROL_ADVANCE) {
        success = addEffectBehavior(CHANGE_CONTROL_ADVANCE,[piece,effect_instance]() {
            for (const auto& e : piece->getActive_effects()) {
                if (e.effect == IMMUNITY_AOE ) {
                    piece->activateEffect(e.effect);
                    return false;
                }
            }
            piece->addEffectStatus(effect_instance);
            piece->setIsWhite(not piece->getIsWhite());
            return true;
        });
    }
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
            if (validTargetGettingEffect(caster_piece,target_piece,effect_instance) && isEffectTargetInGrid(target_piece)) {
                if (configureEffectHandler(target_piece,effect_instance)) {
                    NB_targetTouched++;
                    executeEffect(effect_instance.getEffect(), target_piece);
                    std::cout << "Effect " << Effect_List_to_string[effect_instance.getEffect()] << " applied to piece at (" << targetX << ", " << targetY << ")." << std::endl;
                }

                if (effect_instance.getNB_Target() != -1)
                    CNT_target++;
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
        Pieces* target_piece =  Chessboard::getInstance()->getGrid()[targetX][targetY];
        if (validTargetGettingEffect(caster_piece,target_piece,effect_instance) && isEffectTargetInGrid(target_piece)
            && targetX == range.first && targetY == range.second) {
            if (configureEffectHandler(target_piece,effect_instance)) {
                NB_targetTouched++;
                executeEffect(effect_instance.getEffect(), target_piece);
                std::cout << "Effect " << Effect_List_to_string[effect_instance.getEffect()] << " applied to piece at (" << targetX << ", " << targetY << ")." << std::endl;
            }
        }
    }
    return NB_targetTouched;
}