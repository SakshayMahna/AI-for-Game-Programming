#ifndef AI_H
#define AI_H

#include <cmath>
#include "kinematic.h"

Steering manual_aim(Kinematic target, Kinematic character);
Steering calculated_aim(Kinematic target, Kinematic character);

class TargetIntelligence {
private:
    float max_angle = M_PI / 4;
    float min_angle = 0;
    float angle;

    float max_angle_range = -1;
    float min_angle_range = -1;
    float current_range;

    int trial = 0;

public:
    TargetIntelligence() = default;
    Steering calculated_aim(Kinematic target, Kinematic character);

    void refine_targeting(float target, float range);
};

#endif