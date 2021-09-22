#include "kinematic.h"

// Kinematic constructor
Kinematic::Kinematic(float i_position[2], float i_velocity[2], 
                     float i_orientation, float i_rotation) {
    // Initialize position and orientation
    position[0] = i_position[0];
    position[1] = i_position[1];
    orientation = i_orientation;

    // Initialize velocity and rotation
    velocity[0] = i_velocity[0];
    velocity[1] = i_velocity[1];
    rotation = i_rotation;
}


// Kinematic Update
void Kinematic::update(Steering steering, float time) {
    // Update position and orientation
    position[0] += velocity[0] * time;
    position[1] += velocity[1] * time;
    orientation += rotation * time;

    // Update velocity and rotation
    velocity[0] += steering.linear[0] * time;
    velocity[1] += steering.linear[1] * time;
    orientation += steering.angular * time;
}