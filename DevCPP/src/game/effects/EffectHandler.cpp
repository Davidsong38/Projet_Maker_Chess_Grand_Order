//
// Created by david on 25/01/2025.
//

#include "EffectHandler.h"
#include "Chessboard.h"
#include <utility>


unordered_map<Effect_List, function<void()>> EffectHandler::effectBehaviors;

void EffectHandler::executeEffect(Effect_List effect,Pieces* target_piece) {
    if (effectBehaviors.find(effect) != effectBehaviors.end()) {
        effectBehaviors[effect]();
        target_piece->activateEffect(effect);
    }
}

void EffectHandler::addEffectBehavior(Effect_List effect, function<void()> behavior) {
        effectBehaviors[effect] = std::move(behavior);
}

void EffectHandler::configureEffectHandler(Chessboard &board, Pieces *piece) {
        addEffectBehavior(ALLY_TELEPORT, [&board,piece]() {
        int toX = 0;
        int toY = 0;
        std::cout << "donnes les coord!!!";
        std::cin >> toX >> toY;
        if (toX >= 0 && toX < board.getGrid().size() && toY >= 0 && toY < board.getGrid().size()) {
            piece->setPosition(toX, toY);
        }


    });

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

bool EffectHandler::isEffectTargetInGrid(Pieces * target_piece, Chessboard &board) {
    int coordX = target_piece->getCoordX();
    int coordY = target_piece->getCoordY();
    if (coordX >= 0 && coordX < board.getGrid().size() && coordY >= 0 && coordY < board.getGrid().size()) {
        return true;
    }
    return false;
}

void EffectHandler::applyEffectToTargets(Pieces *caster_piece, EffectInstance effect_instance, Chessboard &board) {
    vector<pair<int,int>> effect_range = caster_piece->getEffectRange(effect_instance.getEffect());
    for (const auto &range: effect_range) {
        int targetX = range.first;
        int targetY = range.second;
        Pieces* target_piece =  board.getGrid()[targetX][targetY];
        if (validTargetGettingEffect(caster_piece,target_piece,effect_instance) && isEffectTargetInGrid(target_piece,board)) {
            if (isTriggerEffect(effect_instance.getEffect())) {
                executeEffect(effect_instance.getEffect(), target_piece);
            } else {
                target_piece->addEffectStatus(effect_instance);
            }
            std::cout << "Effect " << Effect_List_to_string[effect_instance.getEffect()] << " applied to piece at (" << targetX << ", " << targetY << ")." << std::endl;
        }
    }
}
