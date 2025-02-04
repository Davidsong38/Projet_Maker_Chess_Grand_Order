//
// Created by sebas on 02/02/2025.
//

#include "keys.h"
#include "platform.h"

Key* keys[KEY_COUNT];
KeyCodeID KeyCodeLookupTable[KEY_COUNT];

void keys_init() {
  platform_fill_keycode_lookup_table();
  for (auto & key : keys) {
    key = new Key();
  }
}

void update_key(const KeyCodeID key_code_id, const bool isDown) {
  keys[key_code_id]->setKey(isDown);
}

void keys_update() {
  for (auto & key : keys) {
    key->updateKey();
  }
}

Key* get_key(const KeyCodeID key_code_id) {
  return keys[key_code_id];
}