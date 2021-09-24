#ifndef KINEMATIC_H
#define KINEMATIC_H

// Steering Information
struct Steering {
    float linear[2];
};

// Physics Class
class Kinematic {
private:
    float position[2];
    float velocity[2];

public:
    Kinematic(float i_position[2], float i_velocity[2]);
    Kinematic() = default;

    void update(Steering steering, float time);
    void updatePosition(float d_x, float d_y, float mod_x, float mod_y);

    float* getPosition() { return position; }
    float* getVelocity() { return velocity; }
};

#endif