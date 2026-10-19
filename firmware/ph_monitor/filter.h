#pragma once

// Smooths a stream of raw sensor voltages: a short median window rejects
// single-sample spikes (e.g. a bubble), then exponential smoothing shapes
// the response. Pure C++, no Arduino dependency — unit-tested on the host.
class PhFilter {
public:
    PhFilter();

    // Feed one new raw sample, get back the filtered value.
    float update(float rawSample);
};
