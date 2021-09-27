#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdlib>
#include "character.h"
#include "kinematic.h"

float const FACE_WIDTH = 30.f;
float const FACE_HEIGHT = 2.f;
float const RADIUS = 10.f;

// Character constructor
Character::Character(float i_position[2], float i_orientation, sf::Color color) {
    // Initialize Kinematic State
    float i_velocity[] = {0.0, 0.0};
    float i_rotation = 0;
    state = Kinematic(i_position, i_velocity, i_orientation, i_rotation);

    // Initialize SFML Entities
    entity = sf::CircleShape(RADIUS);
    entity.setPosition(i_position[0], i_position[1]);
    entity.setFillColor(color);

    entity_face = sf::RectangleShape(sf::Vector2f(FACE_WIDTH, FACE_HEIGHT));
    entity_face.setPosition(i_position[0] + RADIUS + FACE_HEIGHT / 2, i_position[1] + RADIUS - FACE_HEIGHT / 2);
    entity_face.setFillColor(color);
    entity_face.rotate(i_orientation);
}

// Character Update
void Character::updateState(Steering steering, float time) {
    // Update state
    state.update(steering, time);

    // Update position and orientation
    float* position = state.getPosition();
    float orientation = state.getOrientation();
    entity.setPosition(position[0], position[1]);
    entity_face.setPosition(position[0] + RADIUS + FACE_HEIGHT / 2, position[1] + RADIUS - FACE_HEIGHT / 2);
    entity_face.setRotation(orientation);
}

// Character Position Update
void Character::updatePosition(float d_s, float d_o) {
    // Calculate d_x and d_y
    float o = state.getOrientation();
    float d_x = d_s * cos((o + d_o) * M_PI / 180);
    float d_y = d_s * sin((o + d_o) * M_PI / 180);

    // Update position
    state.updatePosition(d_x, d_y, d_o);
    entity.move(d_x, d_y);
    entity_face.move(d_x, d_y);
    entity_face.rotate(d_o);
}

// Character render
void Character::render(sf::RenderWindow &window) {
    window.draw(entity);
    window.draw(entity_face);
}

// Obstacles Constructor
Obstacles::Obstacles(float width, float height) {
    float x, y;
    
    for (int o = 0; o < N_OBSTACLES; o += 1) {
        x = ((double) rand() / (RAND_MAX)) * width;
        y = ((double) rand() / (RAND_MAX)) * (height - 100);

        obstacles[o] = sf::RectangleShape();
        obstacles[o].setPosition(x, y);
        obstacles[o].setSize(sf::Vector2f(30.f, 10.f));
        obstacles[o].setFillColor(sf::Color::Black);

        float i_velocity[] = {0.0, 0.0}, i_position[] = {x + 15, y + 5};
        float i_rotation = 0, i_orientation = 0;
        states[o] = Kinematic(i_position, i_velocity, i_orientation, i_rotation);
    }

    float l_velocity[] = {0.0, 0.0}, l_position[] = {0, 0};
    float l_rotation = 0, l_orientation = 0;
    states[N_OBSTACLES] = Kinematic(l_position, l_velocity, l_orientation, l_rotation);

    float r_velocity[] = {0.0, 0.0}, r_position[] = {width, 0};
    float r_rotation = 0, r_orientation = 0;
    states[N_OBSTACLES + 1] = Kinematic(r_position, r_velocity, r_orientation, r_rotation);
}

// Obstacles render
void Obstacles::render(sf::RenderWindow &window) {
    for (int o = 0; o < N_OBSTACLES; o += 1)
        window.draw(obstacles[o]);
}