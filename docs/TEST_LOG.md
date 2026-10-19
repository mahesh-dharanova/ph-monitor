# Test Log

Dated hardware-in-loop verification entries. Never edit a past entry — append a new one.

## 2026-10-19 — Chunk 1 scaffold

- `scripts/compile.sh` succeeds against `arduino:renesas_uno:unor4wifi` with the empty
  orchestrator (`Serial.begin()` only) and all stub modules compiled alongside it.
- `tests/run_tests.sh` passes (asserts stub placeholder behavior only — not real logic).
- No hardware connected yet (`arduino-cli board list` shows no serial port); no sensor
  wiring performed.

## 2026-10-19 — Chunk 2: sensor wired and hardware-verified

- Wired PH-4502C: V+→5V, G→GND, PO→A0 (`TO`/`DO` left unconnected). Board:
  `/dev/ttyACM0` (Arduino Uno R4 WiFi).
- `sensor.cpp` implemented for real (windowed ADC average over `SAMPLE_WINDOW_US`,
  raw-to-volts conversion at 14-bit/`ADC_VREF_VOLTS`). `.ino` prints raw sensor output
  directly (`{"ms":...,"raw":...,"v":...}`), same pattern as the sibling
  `turbidity-monitor`/`water-level-monitor` Chunk 2 sessions.
- First attempt: probe tip was wired to the PH-4502C module but the actual glass pH
  electrode (the pen-like probe) was not plugged into the module's BNC jack. Readings
  floated noisily between ~0.71V and ~0.79V with no settling — a floating-input signature,
  not a real pH signal. Root-caused before logging any reading as a baseline.
- After connecting the probe to the module's BNC jack:
  - **Dry (in air)**: stable at **~1.486V** (raw ~4868–4879 @ 14-bit).
  - **In distilled water** (cheap/off-the-shelf, not a pH reference buffer): settled after
    ~25s to a stable **~1.387–1.389V** (raw ~4544–4550) — down from the dry baseline by
    ~0.1V, a clean and repeatable transition.
- Confirms: wiring is correct, `sensor.cpp`'s real ADC read works, and the sensor responds
  to immersion with a distinct, stable signal.
- `scripts/compile.sh` and `tests/run_tests.sh` both still pass after the sensor.cpp change.

### Open items carried forward

- Both observed voltages (dry ~1.486V, in water ~1.387V) are far from the PH-4502C's
  vendor-typical ~2.5V neutral point assumed in `config.h`'s `PH_NEUTRAL_VOLTS`/
  `PH_SLOPE_V_PER_PH` placeholders — expected, since those constants are generic and this
  physical unit hasn't been calibrated. Distilled water is also not a real pH reference (its
  near-zero conductivity makes cheap-probe readings in it unreliable for anything beyond
  "did the signal change") — Phase 2 calibration still needs real pH 4/7/9-ish buffer
  solutions, not distilled water.
- Exact ADC resolution returned by `analogReadResolution(14)` on this board not
  independently double-checked against a known reference voltage yet (shared open item
  with `turbidity-monitor`).
