//
// Created by david on 25/01/2025.
//

#include <iostream>

#include "effects.h"

#include <log.h>

string Effect_List_to_string[EFFECT_COUNT];
bool isBuff_List[EFFECT_COUNT];
bool isTriggerEffect_List[EFFECT_COUNT];

void loadEffectList() {
    Effect_List_to_string[STUN] = "stun";
    Effect_List_to_string[CHANGE_CONTROL] = "changeControl";
    Effect_List_to_string[CHANGE_CONTROL_ADVANCE] = "changeControlAdvance";
    Effect_List_to_string[AOE] = "aoe";
    Effect_List_to_string[GIVING_AOE] = "giveAoe";
    Effect_List_to_string[SHIELD] = "shield";
    Effect_List_to_string[IMMUNITY_EFFECT] = "immunityEffect";
    Effect_List_to_string[IMMUNITY_AOE]  = "immunityAoe";
    Effect_List_to_string[SPAWN_PIECES] = "spawnPieces";
    Effect_List_to_string[ONE_MORE_MOVE] = "oneMoreTurn";
    Effect_List_to_string[SUPP_MOVE] = "suppMove";
    Effect_List_to_string[ALTERNATE_RANGE] = "alternateRange";
    Effect_List_to_string[SUPP_RANGE] = "suppRange";
    Effect_List_to_string[MOVE_CHANGING] = "moveChanging";
    Effect_List_to_string[SWITCHING_PLACE] = "switchingPlace";
    Effect_List_to_string[IMMORTALITY] = "immortality";
    Effect_List_to_string[SACRIFICE] = "sacrifice";
    Effect_List_to_string[KILLING] = "killing";
    Effect_List_to_string[ALLY_TELEPORT] = "ally teleport";
    Effect_List_to_string[ENEMY_TELEPORT] = "enemy teleport";
    Effect_List_to_string[EVOLVE] = "evolve";
    Effect_List_to_string[SUPP_LUCK] = "suppLuck";

    isBuff_List[STUN] = false;
    isBuff_List[CHANGE_CONTROL] = false;
    isBuff_List[CHANGE_CONTROL_ADVANCE] = false;
    isBuff_List[AOE] = false;
    isBuff_List[GIVING_AOE] = true;
    isBuff_List[SHIELD] = true;
    isBuff_List[IMMUNITY_EFFECT] = true;
    isBuff_List[IMMUNITY_AOE]  = true;
    isBuff_List[SPAWN_PIECES] = true;
    isBuff_List[ONE_MORE_MOVE] = true;
    isBuff_List[SUPP_MOVE] = true;
    isBuff_List[ALTERNATE_RANGE] = true;
    isBuff_List[SUPP_RANGE] = true;
    isBuff_List[MOVE_CHANGING] = true;
    isBuff_List[SWITCHING_PLACE] = true;
    isBuff_List[IMMORTALITY] = true;
    isBuff_List[SACRIFICE] = true;
    isBuff_List[KILLING] = false;
    isBuff_List[ALLY_TELEPORT] = true;
    isBuff_List[ENEMY_TELEPORT] = false;
    isBuff_List[EVOLVE] = true;
    isBuff_List[SUPP_LUCK] = true;

    isTriggerEffect_List[STUN] = false;
    isTriggerEffect_List[CHANGE_CONTROL] = true;
    isTriggerEffect_List[CHANGE_CONTROL_ADVANCE] = true;
    isTriggerEffect_List[AOE] = true;
    isTriggerEffect_List[GIVING_AOE] = false;
    isTriggerEffect_List[SHIELD] = false;
    isTriggerEffect_List[IMMUNITY_EFFECT] = false;
    isTriggerEffect_List[IMMUNITY_AOE]  = false;
    isTriggerEffect_List[SPAWN_PIECES] = true;
    isTriggerEffect_List[ONE_MORE_MOVE] = true;
    isTriggerEffect_List[SUPP_MOVE] = true;
    isTriggerEffect_List[ALTERNATE_RANGE] = false;
    isTriggerEffect_List[SUPP_RANGE] = false;
    isTriggerEffect_List[MOVE_CHANGING] = false;
    isTriggerEffect_List[SWITCHING_PLACE] = true;
    isTriggerEffect_List[IMMORTALITY] = false;
    isTriggerEffect_List[SACRIFICE] = true;
    isTriggerEffect_List[KILLING] = true;
    isTriggerEffect_List[ALLY_TELEPORT] = true;
    isTriggerEffect_List[ENEMY_TELEPORT] = true;
    isTriggerEffect_List[EVOLVE] = true;
    isTriggerEffect_List[SUPP_LUCK] = false;
}

bool EffectInstance::isInfinite() const {
    return effect_amount == -1 && effect_duration == -1;
}

bool EffectInstance::isExpired() const {
    return effect_amount == 0 || effect_duration == 0;
}

bool EffectInstance::isBuff() const {
    return isBuff_List[effect];
}

void EffectInstance::activation()  {
    if (effect_amount <= 0)
        return;
    effect_amount--;
    ltr_log_info("EffectInstance::activation(), amount remaining : ", effect_amount);
}

void EffectInstance::decrement_duration() {
    if (effect_duration <= 0)
        return;
    effect_duration--;
}

void EffectInstance::copyTargets(const EffectInstance* effect_instance) {
    this->target_pieces = effect_instance->target_pieces;
    this->target_cells = effect_instance->target_cells;
}
