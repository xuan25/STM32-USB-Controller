#include "led.h"

void LED_SetState(LED* led, GPIO_PinState state) {
  HAL_GPIO_WritePin(led->GPIOx, led->GPIO_Pin, state);
}
