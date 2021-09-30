#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "kinematic.h"

class Character
{
private:
    Kinematic state;

public:
    sf::RectangleShape entity;

    Character(float i_position[2], float i_orientation, sf::Color color);
    Character() = default;

    void updateState(Steering steering, float time);
    void updatePosition(float d_x, float d_y);

    Kinematic getState() { return state; }
};

class MouseAim {
private:
    Kinematic state;
    Kinematic base_state;

public:
    sf::RectangleShape entity;
    
    MouseAim(float i_position[2], float i_orientation, sf::Color color);
    MouseAim() = default;
    void updatePosition(float x, float y);

    Kinematic getState() { return state; };
};

class Projectile {
private:
    Kinematic state;

public:
    sf::CircleShape entity;

    Projectile(float i_position[2], float i_velocity[2], sf::Color color);
    Projectile() = default;
    void update(float dt, int drag);

    Kinematic getState() { return state; };
    float getHeight();
    float getRange();
};

#endif