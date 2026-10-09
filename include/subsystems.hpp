#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

// Your motors, sensors, etc. should go here.  Below are examples

inline pros::Motor intake(-10);
inline pros::Motor toggle(-9);
inline pros::Motor winch1(-8);
inline pros::Motor winch2(1);
inline pros::Motor arm1(7);
inline void winch(int input) {
  winch1.move(input);
  winch2.move(input);
}


inline pros::adi::DigitalOut claw('A'); // Makes Claw
inline pros::Rotation winch_rotation(5); //  Makes arm rotation sensor




// inline pros::adi::DigitalIn limit_switch('A');