#include "calibration.h"

PhEstimate calibrationEstimate(float filteredVolts) {
    (void)filteredVolts;
    // TODO(Chunk 5): vendor-typical neutral-voltage/slope curve, always flagged
    // uncalibrated until Phase 2 supplies real buffer-solution reference points.
    return PhEstimate{ 0.0f, false };
}
