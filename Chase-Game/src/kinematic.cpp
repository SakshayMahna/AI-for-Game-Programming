#include <cmath>
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
    rotation += steering.angular * time;
}

// Kinematic Position Update
void Kinematic::updatePosition(float d_x, float d_y, float mod_x, float mod_y) {
    // Update position
    position[0] += d_x;
    position[1] += d_y;

    // Modulo Update
    position[0] = fmod(position[0] + mod_x, mod_x);
    position[1] = fmod(position[1] + mod_y, mod_y);
}