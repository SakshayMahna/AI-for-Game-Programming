#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "kinematic.h"

// Character Class
class Character {
private:
    Kinematic state;
    float window_width;
    float window_height;

public:
    sf::CircleShape entity;

    Character(float i_position[2], sf::Color color, float width, float height);
    Character() = default;

    void updateState(Steering steering, float time);
    void updatePosition(float d_x, float d_y);

    Kinematic getState() { return state; }
};

#endif