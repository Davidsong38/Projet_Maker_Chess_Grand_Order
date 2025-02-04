//
// Created by david on 25/01/2025.
//

#ifndef EFFECT_LIST_H
#define EFFECT_LIST_H

#include <string>

using namespace std;



enum Effect_List{
     STUN,
     CHANGE_CONTROL,
     AOE,
     GIVING_AOE,
     SHIELD,
     IMMUNITY_EFFECT,
     IMMUNITY_AOE,
     SPAWN_PIECES,
     ONE_MORE_MOVE,
     SUPP_MOVE,
     SUPP_RANGE,
     MOVE_CHANGING,
     SWITCHING_PLACE,
     IMMORTALITY,
     SACRIFICE,
     KILLING,
     ALLY_TELEPORT,
     ENEMY_TELEPORT,
     EVOLVE,
     SUPP_LUCK,
     EFFECT_COUNT,


};

extern string Effect_List_to_string[EFFECT_COUNT];
extern bool isBuff_List[EFFECT_COUNT];
extern bool isTriggerEffect_List[EFFECT_COUNT];

void loadEffectList();


struct EffectInstance {
    Effect_List effect;
    int effect_duration;
    int effect_amount;

    explicit EffectInstance(Effect_List effect, int effect_duration = -1, int effect_amount = -1);

    [[nodiscard]] Effect_List getEffect() const {
        return effect;
    }

    [[nodiscard]] int getEffectDuration() const {
        return effect_duration;
    }

    [[nodiscard]] int getEffectAmount() const {
        return effect_amount;
    }

    [[nodiscard]] bool isInfinite() const;
    [[nodiscard]] bool isExpired() const;
    void activation();
    void decrement_duration();

};


#endif //EFFECT_LIST_H
