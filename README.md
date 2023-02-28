# STM32-USB-HID-Controller

System Controller with STM32 via USB-HID

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

## Default setup in main.c

- KEY_0 (PE4): Consumer Control - Play/Pause
- KEY_1 (PE3): Consumer Control - Scan Next Track
- LED_2 (PF10): key-down indicator

## TODO

- Define Composite USB Device
- Integrate MIDI CC
