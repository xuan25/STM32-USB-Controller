#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

typedef struct Key {
  void (*UserData);
  uint8_t State;
  uint32_t LastLevelChangedMs;
  uint8_t LastChangedLevel;
  void (*OnStateChanged)(struct Key* sender, uint8_t oldState, uint8_t newState);
} Key;

#define KEY_DE_JITTERING_MS 10u

void Key_Update(Key* key, uint8_t level);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */
