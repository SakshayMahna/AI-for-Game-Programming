#include <cmath>
#include "seek.h"
#include "kinematic.h"

const float MAX_SPEED = 1.0;

Steering kinematic_seek(Kinematic target, Kinematic character) {
    // Initialize steering output
    Steering steering;

    // Direction to target
    float* target_position = target.getPosition();
    float* character_position = character.getPosition();
    steering.linear[0] = target_position[0] - character_position[0];
    steering.linear[1] = target_position[1] - character_position[1];

    // Normalize
    float total_speed = sqrt(steering.linear[0] * steering.linear[0] + steering.linear[1] * steering.linear[1]);
    steering.linear[0] = (steering.linear[0] / total_speed) * MAX_SPEED;
    steering.linear[1] = (steering.linear[1] / total_speed) * MAX_SPEED;

    // Rotation
    steering.angular = 0;

    return steering;
}