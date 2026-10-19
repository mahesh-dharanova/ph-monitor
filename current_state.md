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
  real (windowed ADC average + volts conversion), hardware-verified: stable dry baseline
  (~1.486V), then a confirmed, repeatable drop to ~1.387–1.389V once the probe was placed
  in distilled water. First attempt caught an unwired-probe condition (electrode not
  plugged into the module's BNC jack, giving a noisy floating ~0.7–0.8V signal) before any
  reading was trusted — see `docs/TEST_LOG.md` for the full dated entry.

## Next

- Chunk 3: filter module — windowed average + median-of-5 smoothing. Pure logic, no
  hardware session strictly required (host tests via `tests/run_tests.sh`).

## Open questions / blockers

- No pH buffer reference solutions yet — Phase 1 stays in `UNCALIBRATED` mode (see
  `plan.md` Phase 2). Distilled water is not a valid pH reference (near-zero conductivity
  makes cheap-probe readings in it unreliable beyond confirming the signal responds).
- PH-4502C's exact neutral-voltage and slope not yet confirmed against the physical unit —
  both observed voltages (dry and in-water) are well below the ~2.5V vendor-typical
  placeholder in `config.h`, expected until real calibration happens.
- `TO` (temperature) and `DO` (digital threshold) pins intentionally unwired in Phase 1 —
  see `architecture.md`.
- Exact ADC resolution returned by `analogReadResolution(14)` not independently
  cross-checked against a known reference voltage yet.
