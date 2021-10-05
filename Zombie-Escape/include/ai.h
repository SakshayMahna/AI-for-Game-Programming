#ifndef AI_H
#define AI_H

#include "kinematic.h"

Steering kinematic_arrive(Kinematic target, Kinematic character);
Steering kinematic_separation(Kinematic target, Kinematic character);

#endif