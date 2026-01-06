**This is a Counter project with a reset**

Setup:
- Device: STM32F4 Discovery Kit
- Framework: CMSIS

Specs:
- The counter value is shown via the LEDs on the device.
- The button on the device is configured to reset the counter when pressed. Note that as long as the button is not released, the counter will not start.

Testing:
- VS Code and PlatformIO is used for IDE.
- Renode is used for the emulation of STM32F4 Discovery Kit. Note that the "platforms/boards/stm32f4_discovery.repl" file is slightly changed behind the scenes, in order to provide support for all 4 LEDs described in UM1472 (Discovery Kit User Manual).