#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "kinematic.h"

class Character {
private:
    Kinematic state;
    float window_width;
    float window_height;

public:
    sf::CircleShape entity;

    Character(float i_position[2], float i_orientation, sf::Color color, 
              float width, float height);
    Character() = default;

    void updateState(Steering steering, float time);
    void updatePosition(float d_x, float d_y);

    Kinematic getState() { return state; }
};

void initialize_player(float width, float height, Character& player);
void initialize_enemies(float width, float height, Character enemies[8]);

#endif