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

### Same session — additional liquids (tap water, salt water, lemon water)

Probe rinsed in plain water between each liquid to avoid carryover. Same firmware, same
board, continuing straight on from the dry/distilled-water readings above.

- **Tap water**: settled (~40s) at **~1.370–1.372V** (raw ~4495–4497) — in the same
  near-neutral cluster as distilled water, as expected.
- **Salt water** (table salt dissolved in tap water): settled (~60s, slower than the plain
  waters) at **~1.422–1.425V** (raw ~4658–4664) — close to the plain-water cluster but
  offset ~0.03–0.05V higher, *not* a large swing. This is the useful negative result: salt
  water is near pH-neutral, so a real glass pH electrode shouldn't react strongly to
  salinity alone, and it didn't — the small offset is consistent with the known
  ionic-strength/liquid-junction-potential effect on pH electrodes in high-conductivity
  solutions, not a fake large pH shift. Supports the sensor behaving like a true pH probe
  rather than a bare conductivity sensor.
- **Lemon water** (diluted lemon juice, acidic): settled (~45s) at **~2.299–2.300V**
  (raw ~7534–7536) — a large, clean jump of roughly +0.9V above the neutral-water cluster
  and well outside the salt-water offset. Strong, repeatable, directional response to an
  actual acidic sample.

Readings across all five conditions this session, lowest to highest voltage:

| Condition | Volts (settled) |
|---|---|
| Tap water | ~1.370–1.372 |
| Distilled water | ~1.387–1.389 |
| Salt water | ~1.422–1.425 |
| Dry (air) | ~1.486 |
| Lemon water | ~2.299–2.300 |

### Open items carried forward

- All observed voltages are far from the PH-4502C's vendor-typical ~2.5V neutral point
  assumed in `config.h`'s `PH_NEUTRAL_VOLTS`/`PH_SLOPE_V_PER_PH` placeholders — expected,
  since those constants are generic and this physical unit hasn't been calibrated. None of
  tap/distilled/salt/lemon water are real pH reference buffers, so this session confirms
  the sensor *responds* distinctly and directionally to acidity, not an actual pH value —
  Phase 2 calibration still needs real pH 4/7/9-ish buffer solutions.
- Dry (in-air) voltage sits inside the neutral-water cluster rather than at an extreme —
  worth treating `health`'s future "plausible range" check as liquid-contact-dependent, not
  assuming air automatically reads as an outlier.
- Exact ADC resolution returned by `analogReadResolution(14)` on this board not
  independently double-checked against a known reference voltage yet (shared open item
  with `turbidity-monitor`).
- Settling time varies by liquid (distilled/tap/lemon ~25–45s, salt water closer to 60s) —
  worth keeping in mind once `health`'s warm-up check (`HEALTH_WARMUP_MS`) is implemented
  for real in Chunk 4; the current 500ms placeholder is far too short for a liquid-change
  settling transient.
