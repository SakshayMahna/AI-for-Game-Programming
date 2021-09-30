#ifndef KINEMATIC_H
#define KINEMATIC_H

// Steering Information
struct Steering {
    float linear[2];
    float angular;
};

// Physics Class
class Kinematic{

private:
    float position[2];
    float velocity[2];
    float orientation;
    float rotation;

public:
    Kinematic(float i_position[2], float i_velocity[2], 
              float i_orientation, float i_rotation);
    Kinematic() = default;

    void update(Steering steering, float time);
    void updatePosition(float d_x, float d_y);

    float* getPosition() { return position; }
    float* getVelocity() { return velocity; }
    float getOrientation()  { return orientation; }
    float getRotation() { return rotation; }
};

#endif