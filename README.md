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
- KEY_1 (PE3): MIDI CC - Track Volume
- LED_2 (PF10): key-down indicator
