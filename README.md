# Claude Code Macro Pad

Firmware for a 3-key USB macro pad that answers [Claude Code](https://claude.com/claude-code) permission prompts with one press. It's built on the **LOLIN (WEMOS) S2 Mini** (ESP32-S2).

You can buy one assembled on Etsy: **[Claude Code Compatible Macro Pad – 3 Key](https://www.etsy.com/listing/4580757299/claude-code-compatible-macro-pad-3-key)**

The code is free and open source under the MIT License. You can modify, remix, and reflash your pad however you like.

---

## What it does

When Claude Code wants to run a command or edit a file, it shows a numbered prompt:

```
❯ 1. Yes
  2. Yes, and don't ask again
  3. No, and tell Claude what to do differently
```

Each key on the pad types one of those numbers:

| GPIO pin | Key sent | Claude Code action              |
|:--------:|:--------:|---------------------------------|
| 5        | `1`      | Yes (allow once)                |
| 3        | `2`      | Yes, and don't ask again        |
| 1        | `3`      | No / deny                       |

The pad shows up as a standard USB keyboard, so it needs no drivers and works on macOS, Windows, and Linux.

## Hardware

- LOLIN / WEMOS **S2 Mini** (ESP32-S2)
- 3 × mechanical key switches
- Each switch is wired between its GPIO pin and **GND**. The firmware enables the internal pull-up resistors, so no external resistors are needed.

## Flashing the firmware

### 1. Set up the Arduino IDE

1. Install the [Arduino IDE](https://www.arduino.cc/en/software) (2.x).
2. Open **Settings** and add this URL to *Additional boards manager URLs*:
   ```
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```
3. Go to **Tools → Board → Boards Manager**, search for **esp32** (by Espressif Systems), and install it.

### 2. Open the sketch

Open `MacroPad/MacroPad.ino` in the Arduino IDE.

### 3. Select the board

- **Tools → Board → esp32 → LOLIN S2 Mini**
- **Tools → USB Mode → USB-OTG (TinyUSB)**, if your core version shows this option

### 4. Put the S2 Mini into bootloader mode

1. Hold the **0** button on the S2 Mini.
2. Press and release the **RST** button.
3. Release **0**.

The board should now appear as a serial port. Select it under **Tools → Port**.

### 5. Upload

Click **Upload**. When it finishes, press **RST** (or unplug and replug the board). The pad will come back as a USB keyboard.

> **Tip:** After flashing, the board runs as a keyboard rather than a serial device, so you'll need to repeat step 4 every time you want to upload new code.

## Customizing

Everything you're likely to change is in the configuration block at the top of [`MacroPad.ino`](MacroPad/MacroPad.ino):

```cpp
MacroKey keys[] = {
  // pin, key
  {  1,  '3', HIGH, 0 },  // No
  {  3,  '2', HIGH, 0 },  // Yes, and don't ask again
  {  5,  '1', HIGH, 0 },  // Yes
};
```

- **Remap a key:** change the character, for example `'y'`.
- **Use different pins:** change the pin number to match your wiring.
- **Add more keys:** add another line to the array. The loop picks it up automatically.
- **Tune debounce:** adjust `DEBOUNCE_MS` if a key double-fires or feels sluggish.

Ideas for modifications:

- Send `Keyboard.press(KEY_ESC)` to interrupt Claude.
- Send `Shift+Tab` to cycle permission modes.
- Type a whole command or prompt with `Keyboard.print("...")`.

See the [ESP32 USB HID Keyboard docs](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/usb.html) for the full API.

## Troubleshooting

| Problem | Fix |
|---|---|
| Board not detected for upload | Enter bootloader mode (hold **0**, tap **RST**, release **0**) and reselect the port. |
| Keys type nothing | Check that the terminal running Claude Code has focus, and press **RST** after flashing. |
| A key types twice | Increase `DEBOUNCE_MS`. |
| Wrong action on a key | Swap the `key` values in the `keys[]` array. |

## Contributing

Pull requests and forks are welcome. If you build something cool with it, open an issue and share it!

## License

[MIT](LICENSE) © 2026 Logan Severance

*This is an independent project and is not affiliated with or endorsed by Anthropic. "Claude" and "Claude Code" are trademarks of Anthropic.*
