# Hardware Wiring

## Board

Arduino Uno R4 WiFi (`arduino:renesas_uno:unor4wifi`). ADC reference 5V.

## Sensor

PH-4502C pH module, 6-pin header: `PO, TO, DO, G, G, V+`.

| Module pin | Uno R4 WiFi pin | Notes |
|---|---|---|
| V+ | 5V | |
| G | GND | either ground pin; the other `G` is left unconnected |
| PO (analog pH signal) | A0 | |
| TO (temperature) | — | unwired in Phase 1, see `architecture.md` |
| DO (digital threshold) | — | unwired in Phase 1, see `architecture.md` |

## Logic-level check

Module and board both run their analog I/O at 5V — no level-shifting needed as currently
understood. Flag immediately if the physical module turns out to be 3.3V-only (per
`Embedded/CLAUDE.md`'s standing rule to proactively flag voltage mismatches).

## To confirm in Chunk 2

- [ ] Physical pin labels on the module match this table.
- [ ] Observed voltage in a neutral-pH reference liquid (or plain water as a rough check)
      vs. acidic/basic samples, recorded in `TEST_LOG.md`.
- [ ] Actual ADC resolution returned by `analogReadResolution()` on this board (shared fact
      with `turbidity-monitor`'s board — confirm once, reuse the number).
