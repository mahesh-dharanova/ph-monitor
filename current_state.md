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

## Next

- Chunk 2: wire the PH-4502C (V+/G/PO), implement `sensor.cpp`, confirm real ADC resolution
  on hardware, log a hardware session in `docs/TEST_LOG.md`. No board is connected yet
  (`arduino-cli board list` shows no serial port as of this scaffold) — needs the physical
  unit wired and plugged in before this chunk can be verified.

## Open questions / blockers

- No pH buffer reference solutions yet — Phase 1 stays in `UNCALIBRATED` mode (see
  `plan.md` Phase 2).
- PH-4502C's exact neutral-voltage and slope not yet confirmed against the physical unit.
- `TO` (temperature) and `DO` (digital threshold) pins intentionally unwired in Phase 1 —
  see `architecture.md`.
