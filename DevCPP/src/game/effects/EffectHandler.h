//
// Created by david on 25/01/2025.
//

#ifndef EFFECTHANDLER_H
#define EFFECTHANDLER_H
#include <functional>
#include <unordered_map>
#include "Chessboard.h"
#include "Effect_List.h"
using namespace std;


class EffectHandler {
    private:
        static unordered_map<Effect_List, function<void()>> effectBehaviors;
    public:
        static void executeEffect(Effect_List Effect,Pieces* pieces);
        static void addEffectBehavior(Effect_List effect, function<void()> behavior);
        static void configureEffectHandler(Chessboard& board, Pieces* piece);
        static void applyEffectToTargets(Pieces* caster_piece,EffectInstance effect_instance, Chessboard& board);
        static bool validTargetGettingEffect(Pieces *caster_piece, Pieces * target_piece, EffectInstance effect_instance);
        static bool isEffectTargetInGrid(Pieces * target_piece, Chessboard &board);
        static bool isTriggerEffect(Effect_List effect);
        static bool isBuff(Effect_List effect);
        explicit EffectHandler() = default;
};



#endif //EFFECTHANDLER_H
