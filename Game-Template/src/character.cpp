#include <SFML/Graphics.hpp>
#include "character.h"
#include "kinematic.h"

// Character constructor
Character::Character(float i_position[2], float i_orientation, sf::Color color) {
    // Initialize Kinematic State
    float i_velocity[] = {0.0, 0.0};
    float i_rotation = 0;
    state = Kinematic(i_position, i_velocity, i_orientation, i_rotation);

    // Initialize SFML Entity
    entity = sf::CircleShape(10.f);
    entity.setPosition(i_position[0], i_position[1]);
    entity.setFillColor(color);
}

// Character Update
void Character::updateState(Steering steering, float time) {
    // Update state
    state.update(steering, time);

    // Update position
    float* position = state.getPosition();
    entity.setPosition(position[0], position[1]);
}