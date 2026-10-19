# Current State

Updated once per chunk, right after it is built and verified.
This is a pointer into git history for the next agent, not a duplicate of it.

## Done

- Chunk 1 (scaffold): repo structure, `AGENTS.md`/`README.md`/`plan.md`/`architecture.md`,
  stub modules (`sensor`, `filter`, `health`, `calibration`, `reporting`) with signatures
  only, `tests/run_tests.sh` harness, `scripts/compile.sh`/`upload.sh`, doc stubs. Verified:
  `scripts/compile.sh` compiles the empty orchestrator against
  `arduino:renesas_uno:unor4wifi`; `tests/run_tests.sh` runs (no real assertions yet since
  modules are stubs).

- Chunk 2 (sensor): complete. Wired V+→5V, G→GND, PO→A0 on `/dev/ttyACM0`. `sensor.cpp`
  real (windowed ADC average + volts conversion), hardware-verified across five
  conditions in the same session: dry air (~1.486V), tap water (~1.371V), distilled water
  (~1.388V), salt water (~1.423V), and lemon water (~2.300V). The three near-neutral
  liquids cluster tightly; salt water shows only a small ionic-strength offset (not a false
  pH swing from salinity alone); lemon water (acidic) jumps distinctly by ~+0.9V — a clean,
  repeatable, directional response to real acidity. First attempt caught an unwired-probe
  condition (electrode not plugged into the module's BNC jack, giving a noisy floating
  ~0.7–0.8V signal) before any reading was trusted — see `docs/TEST_LOG.md` for the full
  dated entry.

## Next

- Chunk 3: filter module — windowed average + median-of-5 smoothing. Pure logic, no
  hardware session strictly required (host tests via `tests/run_tests.sh`).

## Open questions / blockers

- No pH buffer reference solutions yet — Phase 1 stays in `UNCALIBRATED` mode (see
  `plan.md` Phase 2). None of tap/distilled/salt/lemon water are valid pH reference
  buffers — they confirm the sensor responds distinctly and directionally, not an actual
  pH value.
- PH-4502C's exact neutral-voltage and slope not yet confirmed against the physical unit —
  all observed voltages are well below the ~2.5V vendor-typical placeholder in `config.h`,
  expected until real calibration happens.
- Settling time varies by liquid (~25–45s for distilled/tap/lemon, ~60s for salt water) —
  the current `HEALTH_WARMUP_MS` placeholder (500ms) is far too short for this; revisit in
  Chunk 4.
- `TO` (temperature) and `DO` (digital threshold) pins intentionally unwired in Phase 1 —
  see `architecture.md`.
- Exact ADC resolution returned by `analogReadResolution(14)` not independently
  cross-checked against a known reference voltage yet.
