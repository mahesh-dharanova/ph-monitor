# Calibration Procedure (Phase 2)

Not started — no pH buffer reference solutions on hand yet (see `current_state.md`).

## When buffer solutions are available

1. Record the filtered voltage at each known pH reference point (at minimum pH 7, ideally
   also pH 4 and pH 9/10 to check the slope on both sides of neutral).
2. Fit a line (voltage vs. pH) and put the constants (`PH_NEUTRAL_VOLTS`,
   `PH_SLOPE_V_PER_PH`) in `firmware/ph_monitor/config.h`, consumed by
   `firmware/ph_monitor/calibration.cpp`.
3. Update `tests/test_calibration.cpp` to assert against the fitted points, and flip
   `calibrated` to `true` only once the fit is verified against a held-out sample (not one
   of the fitting points).
4. Record the fit and verification result in `TEST_LOG.md`.
