#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

// Your motors, sensors, etc. should go here.  Below are examples

inline pros::Motor intake(-10);
inline pros::Motor toggle(-9);
inline pros::Motor winch1(-8);
inline pros::Motor winch2(1);

///
// Winch rotation tracking and limits
//
// The V5 motor's integrated encoder reports accumulated degrees (it does NOT
// wrap at 360 / -360).  get_winch_position() reads that raw value so you can
// know exactly how far the winch has traveled.
//
// Call winch_set_limits(lower, upper) in initialize() to define the range of
// motion (in encoder degrees).  The winch() helper will then clamp motor
// output so the winch cannot exceed those bounds.
//
// Defaults are effectively disabled (±1 000 000 000) so the existing behaviour
// is unchanged until you configure limits.
///
inline double WINCH_UPPER_LIMIT = 1000000000.0;  // max allowed degrees (fully raised)
inline double WINCH_LOWER_LIMIT = -1000000000.0; // min allowed degrees (fully lowered)

// Returns the current winch position in degrees (accumulated, not wrapped at 360).
// Uses winch2 (port 1, non-reversed) as the reference encoder because its
// get_position() sign directly corresponds to physical winch direction.
inline double get_winch_position() {
  return winch2.get_position();
}

// Configure the upper and lower limits for winch rotation (in degrees).
// Position is the accumulated encoder count (can exceed 360 / -360).
// Example: winch_set_limits(-200, 1200);
inline void winch_set_limits(double lower_limit, double upper_limit) {
  WINCH_LOWER_LIMIT = lower_limit;
  WINCH_UPPER_LIMIT = upper_limit;
}

// Reset the winch encoder position to 0 degrees on both motors.
inline void winch_reset_encoder() {
  winch1.tare_position();
  winch2.tare_position();
}

// Move the winch (both motors) while respecting configured rotation limits.
// Positive input raises the winch; negative input lowers it.
// If a limit is reached the motor is held (brake) instead of fighting the limit.
inline void winch(int input) {
  double position = get_winch_position();

  // Prevent upward motion past the upper limit
  if (input > 0 && position >= WINCH_UPPER_LIMIT) {
    input = 0;
  }
  // Prevent downward motion past the lower limit
  if (input < 0 && position <= WINCH_LOWER_LIMIT) {
    input = 0;
  }

  winch1.move(input);
  winch2.move(input);
}

// inline pros::adi::DigitalIn limit_switch('A');