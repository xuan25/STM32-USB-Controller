#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

typedef struct LED {
  GPIO_TypeDef* GPIOx;
  uint16_t GPIO_Pin;
} LED;

void LED_SetState(LED* led, GPIO_PinState state);

#ifdef __cplusplus
}
#endif

#endif /* __LED_H */
