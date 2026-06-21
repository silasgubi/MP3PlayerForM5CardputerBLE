# M5Cardputer MP3 Player — EQ Fork

A fully featured, standalone MP3 player for the **M5Stack Cardputer**, forked from [sanchitminda/MP3PlayerForM5Cardputer](https://github.com/sanchitminda/MP3PlayerForM5Cardputer).

This fork adds a **7-band software equalizer** with real-time DSP applied to every audio sample.

---

## What's new in this fork

### 7-Band Equalizer

Press **`E`** from the player screen to open the EQ:

```
┌──────────────────────────────────────┐
│             EQUALIZER        [ON]    │
│  60Hz [====|====]  0dB  <- selected │
│ 170Hz [====|====]  0dB              │
│ 310Hz [========|]  +5dB             │
│ 600Hz [====|====]  0dB              │
│  3kHz [====|====]  0dB              │
│  6kHz [==|======] -3dB              │
│ 12kHz [====|====]  0dB              │
│  ;/.:band   ,//:dB   R:reset  `:exit│
└──────────────────────────────────────┘
```

| Band | Type | Frequency |
|---|---|---|
| Band 1 | Low Shelf | 60 Hz |
| Band 2 | Peak | 170 Hz |
| Band 3 | Peak | 310 Hz |
| Band 4 | Peak | 600 Hz |
| Band 5 | Peak | 3 kHz |
| Band 6 | Peak | 6 kHz |
| Band 7 | High Shelf | 12 kHz |

**EQ Controls:**

| Key | Action |
|---|---|
| `E` (player) | Open EQ screen |
| `;` / `.` | Navigate between bands |
| `,` / `/` | -1 dB / +1 dB (range: -12 to +12) |
| `R` | Reset all bands to 0 dB |
| `T` | Toggle EQ on/off |
| `` ` `` | Save and return to player |

Settings are persisted to NVS flash and survive reboots.

**Implementation:** Biquad IIR filters (direct form II transposed) — 5 multiplications per sample per band per channel. At 7 bands, ~70 operations per stereo sample pair, negligible on ESP32-S3 at 240 MHz.

---

## Original Features

- Split-screen playlist + Now Playing panel
- FFT visualizer (Classic Bars, Waveform, Circular Spikes, OFF)
- MP3, FLAC, AAC, WAV playback
- Shuffle, Loop (All / One / None)
- Fast-forward / rewind seek
- ID3 tag metadata display
- Wi-Fi Web UI (STA client or AP host mode) for streaming/downloading
- Power saver modes (160 MHz / 80 MHz CPU underclock)
- Display sleep timer
- Pocket Mode (Button A click combinations)
- Multi-theme UI (Gunmetal Blue, Cyberpunk, Retro Amber, Hacker Green)
- Folder-based playlist mode
- Song search
- i18n language file support
- Configurable UI font size

## Hardware Requirements

- **M5Stack Cardputer** (ESP32-S3)
- MicroSD card (FAT32)
- Audio files (.mp3, .flac, .aac, .m4a, .wav)

## Building with PlatformIO

The `platformio.ini` and all required configuration are included in the repository.

```bash
git clone https://github.com/silasgubi/MP3PlayerForM5CardputerBLE.git
cd MP3PlayerForM5CardputerBLE
pio run
# Binary: .pio/build/m5stack-cardputer/firmware.bin
```

> The ESP8266Audio library is bundled in `lib/ESP8266Audio/`. The custom board definition is in `boards/m5stack-cardputer.json`.

## Libraries

| Library | Source |
|---|---|
| M5Cardputer | M5Stack |
| M5Unified | M5Stack |
| ESP8266Audio | Bundled in `lib/` (Earle F. Philhower, III) |

## Full Controls

| Key | Function |
|---|---|
| `Enter` | Play / Pause selected |
| `;` / `.` | Scroll playlist up / down |
| `[` / `]` | Volume - / + |
| `N` / `B` | Next / Previous song |
| `/` / `,` | Seek forward / backward |
| `S` | Toggle Search |
| `F` | Toggle Shuffle |
| `L` | Toggle Loop (All / One / None) |
| `V` | Toggle Visualizer |
| `E` | Open Equalizer |
| `` ` `` | Open Settings |
| `I` | Open Help |

### Pocket Mode (Button A)
- 1 click: Play/Pause
- 2 clicks: Next
- 3 clicks: Previous

## License

Open source. Original work by [Sanchit Minda](https://github.com/sanchitminda). EQ fork by [silasgubi](https://github.com/silasgubi).
