#pragma once

// Maps a filtered sensor voltage to a pH estimate. Phase 1 has no reference
// data, so `calibrated` is always false — see architecture.md "UNCALIBRATED
// mode" and docs/CALIBRATION_PROCEDURE.md for Phase 2.
struct PhEstimate {
    float ph;
    bool  calibrated;
};

PhEstimate calibrationEstimate(float filteredVolts);
