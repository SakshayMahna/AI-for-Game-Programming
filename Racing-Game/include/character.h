#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include "kinematic.h"

class Character
{
private:
    Kinematic state;
    sf::CircleShape entity;
    sf::RectangleShape entity_face;

public:

    Character(float i_position[2], float i_orientation, sf::Color color);
    Character() = default;

    void updateState(Steering steering, float time);
    void updatePosition(float d_x, float d_y);
    void render(sf::RenderWindow &window);

    Kinematic getState() { return state; };
};

class Obstacles {
private:
    static const int N_OBSTACLES = 20;
    sf::RectangleShape obstacles[N_OBSTACLES];
    Kinematic states[N_OBSTACLES + 2];

public:
    Obstacles(float width, float height);
    Obstacles() = default;

    void render(sf::RenderWindow &window);

    Kinematic* getStates() { return states; }
};

#endif