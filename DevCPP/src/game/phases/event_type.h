//
// Created by david on 23/02/2025.
//

#ifndef EVENT_TYPE_H
#define EVENT_TYPE_H

#include <string>

enum event_type {
  EVENT_MOVE,
  EVENT_KILL,
  EVENT_SPELL_USED,
  EVENT_EFFECT_TRIGGERED,
  EVENT_EFFECT_APPLY,
  EVENT_EFFECT_UPDATE,
  EVENT_EFFECT_END,
  EVENT_EVOLVED,
  EVENT_TYPE_COUNT
};

enum move_type {
  MOVE_NORMAL,
  MOVE_ROQUED,
  MOVE_EN_PASSANT,
  MOVE_SUPPLEMENTARY,
  MOVE_SUPPLEMENTARY_EN_PASSANT,
  MOVE_TELEPORTED,
  MOVE_KNOCK_BACK,
  MOVE_TYPE_COUNT
};

enum kill_types {
  KILL_NORMAL,
  KILL_EN_PASSANT,
  KILL_KILL_EFFECT,
  KILL_AOE_EFFECT,
  KILL_SACRIFICE,
  KILL_TYPE_COUNT
};

extern std::string event_type_to_string[EVENT_TYPE_COUNT];
extern std::string move_type_to_string[MOVE_TYPE_COUNT];
extern std::string kill_type_to_string[KILL_TYPE_COUNT];

void init_event_type();

#endif //EVENT_TYPE_H
