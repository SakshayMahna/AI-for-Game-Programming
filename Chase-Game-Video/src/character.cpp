#include <SFML/Graphics.hpp>
#include <cmath>
#include "character.h"
#include "kinematic.h"

// Character Constructor
Character::Character(float i_position[2], sf::Color color, float width, float height) {
    // Initialize Kinematic State
    float i_velocity[] = {0.0, 0.0};
    state = Kinematic(i_position, i_velocity);

    // Initialize SFML Entity
    entity = sf::CircleShape(10.f);
    entity.setPosition(i_position[0], i_position[1]);
    entity.setFillColor(color);

    // Initialize window height and width
    window_height = height;
    window_width = width;
}

// Character Update
void Character::updateState(Steering steering, float time) {
    // Update state
    state.update(steering, time);

    // Update position with modulo
    float *position = state.getPosition();
    entity.setPosition(fmod(position[0] + window_width, window_width), fmod(position[1] + window_height, window_height));
    state.updatePosition(0, 0, window_width, window_height);
}

// Character Position Update
void Character::updatePosition(float d_x, float d_y) {
    // Update Position
    state.updatePosition(d_x, d_y, window_width, window_height);
    entity.move(d_x, d_y);

    // Modulo Change
    float *position = state.getPosition();
    entity.setPosition(fmod(position[0] + window_width, window_width), fmod(position[1] + window_height, window_height));
}