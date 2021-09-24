#ifndef SEEK_H
#define SEEK_H

#include "kinematic.h"

extern float MAX_SPEED;

Steering kinematic_seek(Kinematic target, Kinematic character);
Steering kinematic_flee(Kinematic target, Kinematic character);
Steering kinematic_pursue(Kinematic target, Kinematic character);
Steering kinematic_evade(Kinematic target, Kinematic character);

#endif
