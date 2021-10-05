#include <cmath>
#include <algorithm>
#include "ai.h"
#include "kinematic.h"

float const MAX_SPEED = 1.0;
float const MAX_ACCELERATION = 1.0;

float const TARGET_RADIUS = 30.0;
float const SLOW_RADIUS = 100.0;
float const TIME_TO_TARGET = 1.0;

float const DECAY_COEFFICIENT = 50.0;
float const DISTANCE_THRESHOLD = 50.0;

Steering kinematic_arrive(Kinematic target, Kinematic character) {
    // Initialize steering output
    Steering steering;

    // Calculate direction and distance to target
    float direction[2], distance;
    float *target_position = target.getPosition();
    float *character_position = character.getPosition();

    direction[0] = target_position[0] - character_position[0];
    direction[1] = target_position[1] - character_position[1];
    distance = sqrt(direction[0] * direction[0] + direction[1] * direction[1]);

    if (distance < TARGET_RADIUS) {
        steering.linear[0] = 0;
        steering.linear[1] = 0;
        return steering;
    }

    // Calculate target speed
    float target_speed = 0;
    if (distance < SLOW_RADIUS)
        target_speed = MAX_SPEED;
    else
        target_speed = MAX_SPEED * distance / SLOW_RADIUS;

    // Calculate target velocity
    float target_velocity[2];
    target_velocity[0] = (direction[0] / distance) * target_speed;
    target_velocity[1] = (direction[1] / distance) * target_speed;

    // Acceleration to get to target velocity
    steering.linear[0] = target_velocity[0] - character.getVelocity()[0];
    steering.linear[1] = target_velocity[1] - character.getVelocity()[1];

    // Normalize acceleration
    float acceleration = sqrt(steering.linear[0] * steering.linear[0] + steering.linear[1] * steering.linear[1]);
    if (acceleration > MAX_ACCELERATION) {
        steering.linear[0] = (steering.linear[0] / acceleration) * MAX_ACCELERATION;
        steering.linear[1] = (steering.linear[1] / acceleration) * MAX_ACCELERATION;
    }

    return steering;
}

Steering kinematic_separation(Kinematic target, Kinematic character) {
    // Initialize steering output
    Steering steering;
    steering.linear[0] = 0;
    steering.linear[1] = 0;

    // Add strength away from target
    float direction[2];
    float *target_position = target.getPosition();
    float *character_position = character.getPosition();
    direction[0] = character_position[0] - target_position[0];
    direction[1] = character_position[1] - target_position[1];

    float distance = sqrt(direction[0] * direction[0] + direction[1] * direction[1]);

    if (distance < DISTANCE_THRESHOLD) {
        // Calculate strength of repulsion
        float strength = std::min(DECAY_COEFFICIENT / (distance * distance), MAX_ACCELERATION);

        // Subtract acceleration
        direction[0] /= distance;
        direction[1] /= distance;
        steering.linear[0] -= strength * direction[0];
        steering.linear[1] -= strength * direction[1];
    }

    return steering;
}