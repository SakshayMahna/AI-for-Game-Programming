#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include "character.h"
#include "kinematic.h"

float const AIMER_SIZE = 100.f;
float const DRAG_COEFFICIENT = 0.07;

// Character constructor
Character::Character(float i_position[2], float i_orientation, sf::Color color) {
    // Initialize Kinematic State
    float i_velocity[] = {0.0, 0.0};
    float i_rotation = 0;
    state = Kinematic(i_position, i_velocity, i_orientation, i_rotation);

    // Initialize SFML Entity
    entity = sf::RectangleShape(sf::Vector2f(30, 10));
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

// Character Position Update
void Character::updatePosition(float d_x, float d_y) {
    // Update position
    state.updatePosition(d_x, d_y);
    entity.move(d_x, d_y);
}

// Mouse Aimer Constructor
MouseAim::MouseAim(float i_position[2], float i_orientation, sf::Color color) {
    // Initialize Kinematic State
    float i_velocity[] = {0.0, 0.0};
    float i_rotation = 0;
    i_position[0] += 12.5;
    i_position[1] += 5;
    state = Kinematic(i_position, i_velocity, i_orientation, i_rotation);
    base_state = Kinematic(i_position, i_velocity, i_orientation, i_rotation);

    // Initialize SFML Entity
    entity = sf::RectangleShape(sf::Vector2f(0, 0));
    entity.setRotation(i_orientation);
    entity.setPosition(i_position[0], i_position[1]);
    entity.setFillColor(color);
}

// Mouse Aimer Update Position
void MouseAim::updatePosition(float x, float y) {
    // Change state
    float *i_position = state.getPosition();
    state.updatePosition(x - i_position[0], y - i_position[1]);

    // Update size and rotation, which inturn
    // changes position
    float *b_position = base_state.getPosition();
    x = x - b_position[0]; y = y - b_position[1];

    float aimer_size = std::min(sqrt(x*x + y*y), (double) AIMER_SIZE);
    entity.setSize(sf::Vector2f(aimer_size, 5));
    entity.setRotation(atan2(y, x) * 180 / M_PI);
}

// Projectile Constructor
Projectile::Projectile(float i_position[2], float i_velocity[2], sf::Color color) {
    // Initialize Kinematic State
    state = Kinematic(i_position, i_velocity, 0, 0);

    // Initialize SFML Entity
    entity = sf::CircleShape(10.f);
    entity.setPosition(i_position[0], i_position[1]);
    entity.setFillColor(color);
}

// Projectile update
void Projectile::update(float dt, int drag) {
    // Define steering
    float *velocity = state.getVelocity();

    Steering gravity;
    gravity.linear[0] = - DRAG_COEFFICIENT * velocity[0] * drag;
    gravity.linear[1] = 9.8;

    // Update state
    state.update(gravity, dt);

    // Update entity position
    float *new_position = state.getPosition();
    entity.setPosition(new_position[0], new_position[1]);
}

// Return Projectile height
float Projectile::getHeight() {
    Kinematic state = getState();
    float *position = state.getPosition();
    return position[1];
}

// Return Projectile range
float Projectile::getRange() {
    Kinematic state = getState();
    float *position = state.getPosition();
    return position[0];
}