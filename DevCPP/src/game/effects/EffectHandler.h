//
// Created by david on 25/01/2025.
//

#ifndef EFFECTHANDLER_H
#define EFFECTHANDLER_H

#include <functional>
#include <unordered_map>
#include "Chessboard.h"
#include "effects.h"
#include "selection.h"

using namespace std;


class EffectHandler {
public:
    static bool validTargetForEffect(const Pieces* target_piece, const EffectInstance *effect_instance);

    static int selectRandomTargetPieces(EffectInstance *effect_instance);
    static int selectRandomTargetDeadPieces(EffectInstance *effect_instance);
    static int selectRandomTargetCells(EffectInstance *effect_instance);
    static int selectRandomTargetEmptyCells(EffectInstance *effect_instance);
    static int selectRandomTargetNonEmptyCells(EffectInstance *effect_instance);
    static bool selectManualTargetCells(EffectInstance *effect_instance, selection_request_type request);
    static bool cellIsInRange(const chessboard_cell* cell, const EffectInstance *effect_instance);

    static bool applyToTargets(EffectInstance *effect_instance);
    static bool applyBuffToSelf(EffectInstance *effect_instance);


    explicit EffectHandler() = default;
private:
    static unordered_map<Effect_List, function<bool()>> effectBehaviors;

    static bool addEffectBehavior(Effect_List effect, function<bool()> behavior);
    static bool configureEffectHandler(EffectInstance* effect_instance);

    static function<bool()> getStunEffect(EffectInstance* effect_instance);
    static function<bool()> getAoeEffect(EffectInstance* effect_instance);
    static function<bool()> getChangeControlEffect(EffectInstance* effect_instance);
    static function<bool()> getImmunityEffect(EffectInstance* effect_instance);
    static function<bool()> getShieldEffect(EffectInstance* effect_instance);

    static function<bool()> getAlternateRangeEffect(EffectInstance *effect_instance);

    static function<bool()> getSuppRangeEffect(EffectInstance* effect_instance);
    static function<bool()> getKillEffect(EffectInstance* effect_instance);
    static function<bool()> getSpawnPieceEffect(EffectInstance* effect_instance);
    static function<bool()> getOneMoreMoveEffect(EffectInstance* effect_instance);
    static function<bool()> getMoveChangingEffect(EffectInstance *effect_instance);
    static function<bool()> getSuppMoveEffect(EffectInstance *effect_instance);
    static function<bool()> getGivingAOEEffect(EffectInstance *effect_instance);
};



#endif //EFFECTHANDLER_H
