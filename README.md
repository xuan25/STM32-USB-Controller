# STM32-USB-HID-Controller

System Controller with STM32 via USB

## Capability

- Keyboard Mapping
- Mouse Mapping
- Consumer Control Mapping
  - Volume Increment
  - Volume Decrement
  - Mute
  - Play/Pause
  - Scan Next Track
  - Scan Previous Track
  - ...
- MIDI CC

## Default setup in main.c

- KEY_0 (PE4): Consumer Control - Play/Pause
  - LED_1 (PF9): Key-pressed indicator
- KEY_1 (PE3): MIDI CC - Toggle button (on Cable 0, Channel 1, Controller 80)
  - LED_2 (PF10): Toggle indicator
