#include <SFML/Graphics.hpp>
#include <cmath>
#include "character.h"
#include "kinematic.h"

// Character constructor
Character::Character(float i_position[2], float i_orientation, sf::Color color,
                     float width, float height) {
    // Initialize Kinematic State
    float i_velocity[] = {0.0, 0.0};
    float i_rotation = 0;
    state = Kinematic(i_position, i_velocity, i_orientation, i_rotation);

    // Initialize SFML Entity
    entity = sf::CircleShape(30.f);
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
    float* position = state.getPosition();
    entity.setPosition(fmod(position[0] + window_width, window_width), fmod(position[1] + window_height, window_height));
    state.updatePosition(0, 0, window_width, window_height);
}

// Character Position Update
void Character::updatePosition(float d_x, float d_y) {
    // Update position
    state.updatePosition(d_x, d_y, window_width, window_height);
    entity.move(d_x, d_y);

    // Modulo Change
    float* position = state.getPosition();
    entity.setPosition(fmod(position[0] + window_width, window_width), fmod(position[1] + window_height, window_height));
}

// Initialize Player
void initialize_player(float width, float height, Character& player) {
    float player_position[2] = {width/2, height/2};
    player = Character(player_position, 0, sf::Color::Green, width, height);
}

// Initialize Enemies
void initialize_enemies(float width, float height, Character enemies[8]) {
    float enemy_position[8][2];

    enemy_position[0][0] = width/4;
    enemy_position[0][1] = height/2;
    enemies[0] = Character(enemy_position[0], 0, sf::Color::Red, width, height);
    
    enemy_position[1][0] = width/4;
    enemy_position[1][1] = height/4;
    enemies[1] = Character(enemy_position[1], 0, sf::Color::Red, width, height);

    enemy_position[2][0] = width/2;
    enemy_position[2][1] = height/4;
    enemies[2] = Character(enemy_position[2], 0, sf::Color::Red, width, height);

    enemy_position[3][0] = 3 * width/4;
    enemy_position[3][1] = height/4;
    enemies[3] = Character(enemy_position[3], 0, sf::Color::Red, width, height);

    enemy_position[4][0] = 3 * width/4;
    enemy_position[4][1] = height/2;
    enemies[4] = Character(enemy_position[4], 0, sf::Color::Red, width, height);

    enemy_position[5][0] = 3 * width/4;
    enemy_position[5][1] = 3 * height/4;
    enemies[5] = Character(enemy_position[5], 0, sf::Color::Red, width, height);

    enemy_position[6][0] = width/2;
    enemy_position[6][1] = 3 * height/4;
    enemies[6] = Character(enemy_position[6], 0, sf::Color::Red, width, height);

    enemy_position[7][0] = width/4;
    enemy_position[7][1] = 3 * height/4;
    enemies[7] = Character(enemy_position[7], 0, sf::Color::Red, width, height);
}