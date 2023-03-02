#include "key.h"

void Key_Update(Key* key, uint8_t level) {
  // Pressed level
  uint32_t tickMs = HAL_GetTick();
  // Level update
  if(key->LastChangedLevel != level) {
    key->LastChangedLevel = level;
    key->LastLevelChangedMs = tickMs;
  }
  // State update
  if(key->State != level && tickMs - key->LastLevelChangedMs > KEY_DE_JITTERING_MS) {
    uint8_t oldState = key->State;
    key->State = level;
    (*key->OnStateChanged)(key, oldState, level);
  }
}
