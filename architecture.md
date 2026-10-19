# Architecture

Design and structure. Edit only on a real design decision.

## Overview

Single Arduino Uno R4 WiFi sensor node. `ph_monitor.ino` is an orchestrator only; all logic
lives in single-concern modules under `firmware/ph_monitor/`. The split exists so everything
that doesn't strictly need the Arduino runtime (`filter`, `health`, `calibration`,
`reporting`) can be unit-tested on the host with plain `g++` — no emulator, no new toolchain
dependency. Only `sensor.*` touches `Arduino.h`/`analogRead()` and can only be verified on
the physical board.

```
sensor.cpp  --raw ADC, voltage-->  filter.cpp  --smoothed voltage-->  health.cpp
                                                                           |
                                                                           v
                                                    calibration.cpp  --pH (flagged)-->  reporting.cpp
                                                                                               |
                                                                                               v
                                                                                     Serial.println (JSON)
```

## Components

- **`sensor`** — reads the PH-4502C's `PO` analog pin, converts raw ADC counts to volts
  using the board's actual ADC resolution and reference voltage. The only module with
  hardware dependence.
- **`filter`** — windowed average (cancels AC mains flicker) + median-of-5 smoothing (drops
  single-sample spikes, e.g. a bubble). Pure function of a value stream.
- **`health`** — plausibility checks on the (filtered) voltage: out-of-range, stuck value,
  still warming up. Independent of calibration state by design (house rule, see below).
- **`calibration`** — maps voltage to a pH estimate. In Phase 1 there is no reference data,
  so every reading is tagged `"calibrated":false` and uses a vendor-typical neutral-voltage
  + slope curve. Phase 2 replaces the curve's constants once real buffer solutions exist.
- **`reporting`** — builds the one-line JSON report into a fixed buffer. Pure formatting,
  no I/O; the `.ino` does the actual `Serial.println`.

## Why only `PO` is wired in Phase 1

The PH-4502C module exposes 6 pins: `PO` (pH analog signal), `TO` (onboard thermistor,
for temperature-compensated pH — real pH drifts with temperature), `DO` (digital threshold
comparator, fixed trip point set by an onboard trimmer), `G`, `G` (two grounds), `V+` (5V).
Phase 1 only needs the raw pH signal, so only `PO` + power/ground are wired:

- `TO` temperature compensation is a Phase 2+ concern once real calibration exists — adding
  it now would mean guessing a compensation formula with no reference data to verify it
  against.
- `DO` is a fixed-threshold comparator output (set by the module's onboard trimmer, not by
  software), which doesn't fit this project's model of a sensor node reporting raw
  measurements rather than a pre-made threshold decision (see `Embedded/CLAUDE.md`'s
  "sensor nodes stay dumb" rule).

## Key decisions

| Decision | Why | Date |
|---|---|---|
| Sensor node reports raw+filtered+flagged values only, no business logic | House rule in `Embedded/CLAUDE.md`: sensor nodes stay dumb, a future hub would own any decision-making | 2026-10-19 |
| `health` and `calibration` are separate modules/signals | House rule in `Embedded/CLAUDE.md`: never merge or infer one from the other | 2026-10-19 |
| Only `PO` wired in Phase 1; `TO`/`DO` left unconnected | No temperature-compensation reference data yet, and `DO`'s fixed threshold doesn't fit the "dumb sensor node" model — see above | 2026-10-19 |
| Phase 1 ships explicit `UNCALIBRATED` mode instead of a guessed calibration | No pH buffer reference solutions on hand; a silently-wrong pH number is worse than an honestly-flagged approximate one | 2026-10-19 |
| Host-side unit tests via plain `g++`, no framework | `filter`/`health`/`calibration`/`reporting` have zero Arduino dependency; g++ is already installed; avoids adding PlatformIO/Unity for no benefit | 2026-10-19 |
| Board: Arduino Uno R4 WiFi, FQBN `arduino:renesas_uno:unor4wifi` | User's hardware; WiFi unused in Phase 1 but available if a later phase needs it | 2026-10-19 |
| Own project, own board — not merged into `turbidity-monitor`/`water-level-monitor` yet | Each sensor node proven independently first; a combined multi-sensor node is a deliberate later decision, not a default (see `plan.md`) | 2026-10-19 |

## Open hardware questions (resolve during Chunk 2, don't assume)

- Exact ADC resolution the Uno R4 WiFi actually returns via `analogReadResolution()`
  (datasheet claims up to 14-bit) — same open question as `turbidity-monitor`; confirm once
  per board, not per project, once either project's Chunk 2 hardware session runs.
- The PH-4502C's actual neutral-point voltage and slope on this specific physical unit —
  `config.h`'s `PH_NEUTRAL_VOLTS`/`PH_SLOPE_V_PER_PH` are vendor-typical placeholders until
  Phase 2 buffer calibration replaces them.
