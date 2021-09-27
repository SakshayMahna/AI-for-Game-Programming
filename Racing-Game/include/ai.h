#ifndef AI_H
#define AI_H

#include "kinematic.h"

Steering kinematic_seek(Kinematic target, Kinematic character);
Steering kinematic_align(Kinematic target, Kinematic character);
Steering kinematic_seek_align(Kinematic target, Kinematic character);
Steering look_where_going(Kinematic target, Kinematic character);

Steering kinematic_separation(Kinematic* obstacles, Kinematic target, Kinematic character);
Steering kinematic_separation_align(Kinematic* obstacles, Kinematic target, Kinematic character);
Steering look_where_going_obstacles(Kinematic* obstacles, Kinematic target, Kinematic character);

#endif