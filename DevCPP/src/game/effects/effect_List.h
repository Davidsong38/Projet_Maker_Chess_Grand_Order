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
     EFFECT_COUNT,


};

extern string Effect_List_to_string[EFFECT_COUNT];
extern bool isBuff_List[EFFECT_COUNT];
extern bool isTriggerEffect_List[EFFECT_COUNT];

void loadEffectList();


struct EffectInstance {
    static EffectInstance* instance;
    Effect_List effect;
    int effect_duration;
    int effect_amount;
    int NB_Target;
    void* caster_piece;


    EffectInstance(Effect_List effect, int effect_duration, int effect_amount, int NB_Target, void* caster_piece)
    {
        this->effect = effect;
        this->effect_duration = effect_duration;
        this->effect_amount = effect_amount;
        this->NB_Target = NB_Target;
        this->caster_piece = caster_piece;
        //std::cout << (long long int)(caster_piece) << endl;

    }

    EffectInstance(Effect_List effect, int effect_duration, int effect_amount, int NB_Target)
    {
        this->effect = effect;
        this->effect_duration = effect_duration;
        this->effect_amount = effect_amount;
        this->NB_Target = NB_Target;
        this->caster_piece = nullptr;

    }

    static EffectInstance* getInstance();

    [[nodiscard]] Effect_List getEffect() const {
        return effect;
    }

    [[nodiscard]] int getEffectDuration() const {
        return effect_duration;
    }

    [[nodiscard]] int getEffectAmount() const {
        return effect_amount;
    }

    [[nodiscard]] int getNB_Target() const {
        return NB_Target;
    }


    void setEffectAmount(int effect_amount){
        this->effect_amount = effect_amount;
    }

    [[nodiscard]] bool isInfinite() const;
    [[nodiscard]] bool isExpired() const;
    void activation();
    void decrement_duration();

};


#endif //EFFECT_LIST_H
