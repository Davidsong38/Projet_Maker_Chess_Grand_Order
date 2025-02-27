//
// Created by david on 25/02/2025.
//

#include "event_type.h"

#include <string>

std::string event_type_to_string[EVENT_TYPE_COUNT];
std::string move_type_to_string[MOVE_TYPE_COUNT];
std::string kill_type_to_string[KILL_TYPE_COUNT];

void init_event_type() {
    event_type_to_string[EVENT_MOVE] = "event_move";
    event_type_to_string[EVENT_KILL] = "event_kill";
    event_type_to_string[EVENT_SPELL_USED] = "event_spell_used";
    event_type_to_string[EVENT_EFFECT_TRIGGERED] = "event_effect_triggered";
    event_type_to_string[EVENT_EFFECT_APPLY] = "event_effect_apply";
    event_type_to_string[EVENT_EFFECT_UPDATE] = "event_effect_update";
    event_type_to_string[EVENT_EFFECT_END] = "event_effect_end";
    event_type_to_string[EVENT_EVOLVED] = "event_evolved";

    move_type_to_string[MOVE_NORMAL] = "move_normal";
    move_type_to_string[MOVE_ROQUED] = "move_roqued";
    move_type_to_string[MOVE_EN_PASSANT] = "move_en_passant";
    move_type_to_string[MOVE_SUPPLEMENTARY] = "move_supplementary";
    move_type_to_string[MOVE_TELEPORTED] = "move_teleported";
    move_type_to_string[MOVE_KNOCK_BACK] = "move_knock_back";

    kill_type_to_string[KILL_NORMAL] = "kill_normal";
    kill_type_to_string[KILL_EN_PASSANT] = "kill_en_passant";
    kill_type_to_string[KILL_KILL_EFFECT] = "kill_effect";
    kill_type_to_string[KILL_AOE_EFFECT] = "kill_effect_aoe";
    kill_type_to_string[KILL_SACRIFICE] = "kill_effect_sacrifice";
}