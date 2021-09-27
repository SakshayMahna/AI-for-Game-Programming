#include <cmath>
#include <algorithm>
#include <iostream>
#include "ai.h"
#include "kinematic.h"

const float MAX_SPEED = 0.02;

const float MAX_ANGULAR_ACCELERATION = 30.0;
const float MAX_ROTATION = 30.0;
const float TARGET_RADIUS = 0.01;
const float SLOW_RADIUS = 0.05;
const float TIME_TO_TARGET = 0.01;

const float DECAY_COEFFICIENT = 1000.0;
const float MAX_ACCELERATION = 50.0;
const float DISTANCE_THRESHOLD = 70.0;

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

Steering kinematic_align(Kinematic target, Kinematic character) {
    // Initialize steering output
    Steering steering;

    // Get direction to target
    float rotation = (target.getOrientation() - character.getOrientation()) * M_PI / 180;

    // Get between (-pi to pi range)
    while (fabs(rotation) > 2 * M_PI) {
        if (rotation < 0)
            rotation += 2 * M_PI;
        else
            rotation -= 2 * M_PI;
    }

    float rotation_size = fabs(rotation);

    // Check if we are done?
    if (rotation_size < TARGET_RADIUS) {
        steering.linear[0] = 0;
        steering.linear[1] = 0;
        steering.angular = 0;
        return steering;
    }

    float target_rotation;

    // If outside slow radius, use maximum rotation
    if (rotation_size > SLOW_RADIUS)
        target_rotation = MAX_ROTATION;
    else
        target_rotation = MAX_ROTATION * rotation_size / SLOW_RADIUS;

    // Combine direction
    target_rotation *= rotation / rotation_size;

    // Calculate steering acceleration
    steering.angular = target_rotation - character.getRotation();
    steering.angular /= TIME_TO_TARGET;

    // Normalize acceleration
    float angular_acceleration = fabs(steering.angular);
    if (angular_acceleration > MAX_ANGULAR_ACCELERATION) {
        steering.angular /= angular_acceleration;
        steering.angular *= MAX_ANGULAR_ACCELERATION;
    }

    // Output steering
    steering.linear[0] = 0;
    steering.linear[1] = 0;
    steering.angular *= 180 / M_PI;

    return steering;
}

Steering kinematic_seek_align(Kinematic target, Kinematic character) {
    // Get seek and align outputs
    Steering seek_output = kinematic_seek(target, character);
    Steering align_output = kinematic_align(target, character);

    // Return a combined steering
    Steering combined_output;
    combined_output.linear[0] = seek_output.linear[0];
    combined_output.linear[1] = seek_output.linear[1];
    combined_output.angular = align_output.angular;

    return combined_output;
}

Steering look_where_going(Kinematic target, Kinematic character) {
    // Check for zero direction, make no change if so
    float *character_velocity = character.getVelocity();
    float character_speed = sqrt(character_velocity[0] * character_velocity[0] + character_velocity[1] * character_velocity[1]);
    if (character_speed < 0.001)
        return kinematic_seek_align(target, character);


    // Otherwise set target based on velocity
    float new_orientation = atan2(character_velocity[1], character_velocity[0]);
    new_orientation = new_orientation * 180 / M_PI;
    Kinematic new_target = Kinematic(target.getPosition(), target.getVelocity(), new_orientation, target.getRotation());

    return kinematic_seek_align(new_target, character);
}

Steering kinematic_separation(Kinematic* obstacles, Kinematic target, Kinematic character) {
    // Initialize steering output
    Steering steering;
    steering.linear[0] = 0;
    steering.linear[1] = 0;
    steering.angular = 0;

    // Loop through each obstacle
    // int n_obstacles = sizeof(obstacles) / sizeof(obstacles[0]);
    float *character_position = character.getPosition();
    for (int o = 0; o < 20; o += 1) {
        Kinematic obstacle = obstacles[o];

        // Check if target is close
        float direction[2];
        float *obstacle_position = obstacle.getPosition();
        direction[0] = obstacle_position[0] - character_position[0];
        direction[1] = obstacle_position[1] - character_position[1];

        float distance = sqrt(direction[0] * direction[0] + direction[1] * direction[1]);

        if (distance < DISTANCE_THRESHOLD) {
            // Calculate strength of repulsion
            float strength = std::min(DECAY_COEFFICIENT / (distance * distance), MAX_ACCELERATION);

            // Subtract acceleration
            direction[0] /= distance;
            direction[1] /= distance;
            steering.linear[0] -= strength * direction[0];
            // steering.linear[1] -= strength * direction[1];
        }
    }

    // Avoid going all the way to left or right
    float *l_position = obstacles[20].getPosition();
    float l_direction = l_position[0] - character_position[0];
    float l_distance = fabs(l_direction);
    float l_strength = std::min(DECAY_COEFFICIENT / (l_direction * l_direction), MAX_ACCELERATION);
    steering.linear[0] -= l_strength * l_distance / l_direction;

    float *r_position = obstacles[21].getPosition();
    float r_direction = r_position[0] - character_position[0];
    float r_distance = fabs(r_direction);
    float r_strength = std::min(DECAY_COEFFICIENT / (r_direction * r_direction), MAX_ACCELERATION);
    steering.linear[0] -= r_strength * r_distance / r_direction;

    // Add strength towards target
    float direction[2];
    float *target_position = target.getPosition();
    direction[0] = target_position[0] - character_position[0];
    direction[1] = target_position[1] - character_position[1];

    float distance = sqrt(direction[0] * direction[0] + direction[1] * direction[1]);

    // Calculate force towards target
    steering.linear[0] += (direction[0] / distance) * MAX_SPEED;
    steering.linear[1] += (direction[1] / distance) * MAX_SPEED;

    return steering;
}

Steering kinematic_separation_align(Kinematic* obstacles, Kinematic target, Kinematic character) {
    // Get seek and align outputs
    Steering separation_output = kinematic_separation(obstacles, target, character);
    Steering align_output = kinematic_align(target, character);

    // Return a combined steering
    Steering combined_output;
    combined_output.linear[0] = separation_output.linear[0];
    combined_output.linear[1] = separation_output.linear[1];
    combined_output.angular = align_output.angular;

    return combined_output;
}

Steering look_where_going_obstacles(Kinematic* obstacles, Kinematic target, Kinematic character) {
    // Check for zero direction, make no change if so
    float *character_velocity = character.getVelocity();
    float character_speed = sqrt(character_velocity[0] * character_velocity[0] + character_velocity[1] * character_velocity[1]);
    if (character_speed < 0.001)
        return kinematic_separation_align(obstacles, target, character);


    // Otherwise set target based on velocity
    float new_orientation = atan2(character_velocity[1], character_velocity[0]);
    new_orientation = new_orientation * 180 / M_PI;
    Kinematic new_target = Kinematic(target.getPosition(), target.getVelocity(), new_orientation, target.getRotation());

    return kinematic_separation_align(obstacles, new_target, character);
}