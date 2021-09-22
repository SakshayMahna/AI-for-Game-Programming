#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "kinematic.h"

class Character
{
private:
    Kinematic state;

public:
    sf::CircleShape entity;

    Character(float i_position[2], float i_orientation, sf::Color color);
    Character() = default;

    void updateState(Steering steering, float time);
    void updatePosition(float i_position[2]);

    Kinematic getState() { return state; }
};

#endif