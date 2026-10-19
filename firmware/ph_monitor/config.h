#pragma once
#include <cstdint>

// Named constants shared by the pure logic modules and sensor.cpp. No magic
// numbers scattered in module bodies — change the behavior here.

// ADC
constexpr uint8_t ADC_RESOLUTION_BITS = 14;    // Uno R4 WiFi's max; confirm in Chunk 2
constexpr float   ADC_VREF_VOLTS      = 5.0f;  // module and board both run at 5V

// Timing
constexpr uint32_t REPORT_INTERVAL_MS = 1000;
constexpr uint32_t SAMPLE_WINDOW_US   = 200000UL; // 200 ms cancels 50/60 Hz mains flicker

// Filter (Chunk 3)
constexpr uint8_t FILTER_WINDOW_SIZE  = 5;
constexpr float   FILTER_SMOOTHING    = 0.4f;     // 0..1, higher = faster response

// Health (Chunk 4)
constexpr float    HEALTH_MIN_VOLTS   = 0.0f;
constexpr float    HEALTH_MAX_VOLTS   = ADC_VREF_VOLTS;
constexpr uint32_t  HEALTH_WARMUP_MS  = 500;

// Calibration (Chunk 5) — vendor-typical PH-4502C placeholders, UNCALIBRATED until
// Phase 2 fits these against real pH buffer solutions (see docs/CALIBRATION_PROCEDURE.md).
constexpr float PH_NEUTRAL_VOLTS     = 2.5f;   // voltage at pH 7 (board midpoint)
constexpr float PH_SLOPE_V_PER_PH    = 0.18f;  // volts per pH unit away from neutral
