#include <cmath>
#include "ai.h"
#include "kinematic.h"

float MAX_SPEED;
const float MAX_PREDICTION = 0.1;

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

Steering kinematic_flee(Kinematic target, Kinematic character) {
    // Initialize steering output
    Steering steering;

    // Direction to target
    float* target_position = target.getPosition();
    float* character_position = character.getPosition();
    steering.linear[0] = character_position[0] - target_position[0];
    steering.linear[1] = character_position[1] - target_position[1];

    // Normalize
    float total_speed = sqrt(steering.linear[0] * steering.linear[0] + steering.linear[1] * steering.linear[1]);
    steering.linear[0] = (steering.linear[0] / total_speed) * MAX_SPEED;
    steering.linear[1] = (steering.linear[1] / total_speed) * MAX_SPEED;

    // Rotation
    steering.angular = 0;

    return steering;
}

Steering kinematic_pursue(Kinematic target, Kinematic character) {
    // Calculate direction and distance to target
    float direction[2], distance;
    float* target_position = target.getPosition();
    float* character_position = character.getPosition();

    direction[0] = target_position[0] - character_position[0];
    direction[1] = target_position[1] - character_position[1];
    distance = sqrt(direction[0] * direction[0] + direction[1] * direction[1]);

    // Calculate speed
    float* character_velocity = character.getVelocity();
    float speed = sqrt(character_velocity[0] * character_velocity[0] + character_velocity[1] * character_velocity[1]);

    // Calculate prediction time
    float prediction;
    if (speed <= distance / MAX_PREDICTION)
        prediction = MAX_PREDICTION;
    else
        prediction = distance / speed;

    // Calculate a new target for kinematic seek algorithm
    float* target_velocity = target.getVelocity();
    float new_target_position[2] = {target_position[0] + target_velocity[0] * prediction, 
                                    target_position[1] + target_velocity[1] * prediction};
    Kinematic new_target = Kinematic(new_target_position, target_velocity, 0, 0);

    return kinematic_seek(new_target, character);
}

Steering kinematic_evade(Kinematic target, Kinematic character) {
    // Calculate direction and distance to target
    float direction[2], distance;
    float* target_position = target.getPosition();
    float* character_position = character.getPosition();

    direction[0] = target_position[0] - character_position[0];
    direction[1] = target_position[1] - character_position[1];
    distance = sqrt(direction[0] * direction[0] + direction[1] * direction[1]);

    // Calculate speed
    float* character_velocity = character.getVelocity();
    float speed = sqrt(character_velocity[0] * character_velocity[0] + character_velocity[1] * character_velocity[1]);

    // Calculate prediction time
    float prediction;
    if (speed <= distance / MAX_PREDICTION)
        prediction = MAX_PREDICTION;
    else
        prediction = distance / speed;

    // Calculate a new target for kinematic seek algorithm
    float* target_velocity = target.getVelocity();
    float new_target_position[2] = {target_position[0] + target_velocity[0] * prediction, 
                                    target_position[1] + target_velocity[1] * prediction};
    Kinematic new_target = Kinematic(new_target_position, target_velocity, 0, 0);

    return kinematic_flee(new_target, character);
}