//
// Created by david on 25/01/2025.
//

#ifndef EFFECTS_H
#define EFFECTS_H

#include <string>
#include <vector>

using namespace std;



enum Effect_List{
     STUN,
     CHANGE_CONTROL,
     CHANGE_CONTROL_ADVANCE,
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
     EFFECT_COUNT
};

extern string Effect_List_to_string[EFFECT_COUNT];
extern bool isBuff_List[EFFECT_COUNT];
extern bool isTriggerEffect_List[EFFECT_COUNT];

void loadEffectList();

struct EffectInstance {
    const Effect_List effect;
    void* caster_piece;

    int effect_duration{-1};
    int effect_amount{-1};
    int NB_Target{-1};

    bool requires_hitting_something{false};

    std::vector<void*> target_pieces;
    std::vector<void*> target_cells;

    EffectInstance(
        const Effect_List effect,
        void* caster_piece
    ) : effect(effect), caster_piece(caster_piece) {}

    EffectInstance(
        const Effect_List effect,
        void* caster_piece,
        const int effect_duration,
        const int effect_amount,
        const int NB_Target
    ) : effect(effect), caster_piece(caster_piece) {
      this->effect_duration = effect_duration;
      this->effect_amount = effect_amount;
      this->NB_Target = NB_Target;
    }

    [[nodiscard]] bool isInfinite() const;
    [[nodiscard]] bool isExpired() const;

    [[nodiscard]] bool isBuff() const;

    void activation();
    void decrement_duration();

    void copyTargets(const EffectInstance* effect_instance);

};


#endif //EFFECTS_H
