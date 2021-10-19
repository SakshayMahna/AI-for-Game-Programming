#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>
#include "character.h"
#include "animation.h"

const float HEALTH_HEIGHT = 20;
const float HEALTH_WIDTH = 1000;
const float HEALTH_OFFSET = 100;

// Character constructor
Character::Character(float i_position[2], sf::Color color, std::string texture_filename, 
                     bool enemy, float h_height) {
    // Initialize attributes
    health = 100;
    health_entity.setFillColor(color);
    health_entity.setPosition(HEALTH_OFFSET, h_height);
    health_entity.setSize(sf::Vector2f(HEALTH_WIDTH * (health / 100), HEALTH_HEIGHT));
    block = false;

    // Initialize position
    position[0] = i_position[0];
    position[1] = i_position[1];

    // Initialize entity and texture
    texture = TextureManager();
    texture.load_texture("animation", texture_filename);

    entity.setPosition(i_position[0], i_position[1]);
    if (enemy)
        entity.setScale(-1.5, 1.5);
    else
        entity.setScale(1.5, 1.5);
        
    entity.setTexture(texture.getRef("animation"));
}

// Character Position Update
void Character::updatePosition(float d_x, float d_y) {
    // Update position
    position[0] += d_x;
    position[1] += d_y;

    entity.move(d_x, d_y);
}

// Initialize animation
void Character::initializeAnimations(int left, int top, int width, int height, std::vector<int> durations) {
    handler = AnimationHandler(sf::IntRect(left, top, width, height));
    
    for (int duration : durations) {
        Animation anim = Animation(1, duration - 1, duration);
        handler.add_animation(anim);
    }

    handler.change_animation(0);
    entity.setTextureRect(handler.bounds);
}

// Animation Function
void Character::animate(int animation_id) {
    handler.change_animation(animation_id);
    handler.update(0.03);

    entity.setTextureRect(handler.bounds);
}

// Render function
void Character::render(sf::RenderWindow &window) {
    window.draw(entity);
    window.draw(health_entity);
}

// Change the health of character
void Character::changeHealth(float dh) {
    health += dh;
    health_entity.setSize(sf::Vector2f(HEALTH_WIDTH * (health / 100), HEALTH_HEIGHT));
}