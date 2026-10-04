/*
 * Claude Code Macro Pad
 * https://github.com/Phishing-Fish/Claude_Code_Macro_Pad
 *
 * A 3-key USB keyboard for answering Claude Code permission prompts.
 * Each key types the number of a menu option in the prompt:
 *
 *   1  ->  Yes
 *   2  ->  Yes, and don't ask again
 *   3  ->  No, and tell Claude what to do differently
 *
 * Board: LOLIN (WEMOS) S2 Mini (ESP32-S2) — uses the chip's native USB HID.
 * Wiring: each switch connects its GPIO pin to GND (internal pull-ups are used).
 *
 * Released under the MIT License — see LICENSE.
 */

#include "USB.h"
#include "USBHIDKeyboard.h"

// ---------------------------------------------------------------------------
// Configuration — edit these to remap keys or change pins.
// ---------------------------------------------------------------------------

struct MacroKey {
  uint8_t pin;   // GPIO the switch is wired to
  char    key;   // character typed when pressed
  // Runtime state (leave as-is)
  int           lastState;
  unsigned long lastPressTime;
};

MacroKey keys[] = {
  // pin, key
  {  1,  '3', HIGH, 0 },  // No
  {  3,  '2', HIGH, 0 },  // Yes, and don't ask again
  {  5,  '1', HIGH, 0 },  // Yes
};

const unsigned long DEBOUNCE_MS = 200;  // ignore repeat presses within this window
const unsigned long LOOP_DELAY_MS = 5;

// ---------------------------------------------------------------------------

const size_t NUM_KEYS = sizeof(keys) / sizeof(keys[0]);

USBHIDKeyboard Keyboard;

void setup() {
  for (size_t i = 0; i < NUM_KEYS; i++) {
    pinMode(keys[i].pin, INPUT_PULLUP);
  }
  Keyboard.begin();
  USB.begin();
}

void loop() {
  unsigned long now = millis();

  for (size_t i = 0; i < NUM_KEYS; i++) {
    MacroKey &k = keys[i];
    int state = digitalRead(k.pin);

    // Fire once on the falling edge (switch pressed), with debounce.
    if (state == LOW && k.lastState == HIGH && (now - k.lastPressTime) > DEBOUNCE_MS) {
      Keyboard.write(k.key);
      k.lastPressTime = now;
    }
    k.lastState = state;
  }

  delay(LOOP_DELAY_MS);
}
