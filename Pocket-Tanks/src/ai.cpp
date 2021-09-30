#include <cmath>
#include <iostream>
#include "ai.h"
#include "kinematic.h"

const float MAX_SPEED = 100.0;

Steering manual_aim(Kinematic target, Kinematic character) {
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

Steering calculated_aim(Kinematic target, Kinematic character) {
    // Initialize steering output and gravity steering
    Steering steering;
    Steering gravity;
    gravity.linear[0] = 0;
    gravity.linear[1] = 9.8;

    // Calculate the distance between target and character
    float *target_position = target.getPosition();
    float *character_position = character.getPosition();
    float distance = target_position[0] - character_position[0];
    
    // Calculate the angle
    float angle = - asin(distance * gravity.linear[1] / (MAX_SPEED * MAX_SPEED)) / 2;
    angle = - M_PI / 2 - angle;
    
    // Calculate the final velocities
    steering.linear[0] = MAX_SPEED * cos(angle);
    steering.linear[1] = MAX_SPEED * sin(angle);
    steering.angular = 0;

    return steering;
}

Steering TargetIntelligence::calculated_aim(Kinematic target, Kinematic character) {
    // First two trials are to calculate the Range of Impact
    // Binary Search kind of setting
    if (trial == 0)
        angle = max_angle;
    else if (trial == 1)
        angle = min_angle;
    else
        angle = (max_angle + min_angle) / 2;

    // Calculate the distance between target and character
    float *target_position = target.getPosition();
    float *character_position = character.getPosition();
    float distance = target_position[0] - character_position[0];

    Steering steering;

    if (distance < 0) {
        steering.linear[0] = MAX_SPEED * cos(-1 * (M_PI / 2 + angle));
        steering.linear[1] = MAX_SPEED * sin(-1 * (M_PI / 2 + angle));
        steering.angular = 0;
    } else {
        steering.linear[0] = MAX_SPEED * cos(angle);
        steering.linear[1] = MAX_SPEED * sin(angle);
        steering.angular = 0;
    }

    trial += 1;

    return steering;
}

void TargetIntelligence::refine_targeting(float target, float range) {
    // First two trials are to calculate the Range of Impact
    if (trial == 1)
        max_angle_range = range;
    else if (trial == 2)
        min_angle_range = range;
    else {
        // Binary Search type of setting
        if (range < target) {
            max_angle_range = range;
            max_angle = angle;
        } else {
            min_angle_range = range;
            min_angle = angle;
        }
    }
}