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
        static unordered_map<Effect_List, function<bool()>> effectBehaviors;
    public:
        static void executeEffect(Effect_List Effect,Pieces* pieces);
        static bool addEffectBehavior(Effect_List effect, function<bool()> behavior);
        static bool configureEffectHandler(int coordX,int coordY,Pieces *piece, EffectInstance effect_instance);
        static int applyEffectToTargets(Pieces *caster_piece, EffectInstance effect_instance);
        static int applyEffectToSelectionnedTarget(Pieces* caster_piece, EffectInstance effect_instance);
        static int applyEffectToSelectionnedTarget(Pieces* caster_piece, EffectInstance effect_instance, int targetX, int targetY);
        static bool applyBuffToSelf(Pieces* caster_piece, EffectInstance effect_instance);
        static bool applyToEmptyCell(Pieces* caster_piece, EffectInstance effect_instance);
        static bool validTargetGettingEffect(Pieces *caster_piece, Pieces * target_piece, EffectInstance effect_instance);
        static bool isEffectTargetInGrid(Pieces *target_piece);
        static bool isEffectTargetInGrid(int coordX, int coordY);
        static bool isTriggerEffect(Effect_List effect);
        static bool isBuff(Effect_List effect);
        explicit EffectHandler() = default;
};



#endif //EFFECTHANDLER_H
