#ifndef AI_H
#define AI_H

#include "kinematic.h"

Steering kinematic_seek(Kinematic target, Kinematic character);
Steering kinematic_pursue(Kinematic target, Kinematic character);

#endif