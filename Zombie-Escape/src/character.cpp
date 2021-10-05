#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>
#include <iostream>
#include <functional>
#include "ai.h"
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
    // entity.setPosition(position[0], position[1]);
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
    // entity.setPosition(position[0], position[1]);
}

// TreeCharacter Constructor
TreeEntity::TreeEntity(float i_position[2], sf::Color color, 
            TreeEntity *left, TreeEntity *right, TreeEntity *parent,
            float width, float height) {
    // Initialize Kinematic State
    float i_velocity[] = {0.0, 0.0};
    Kinematic state = Kinematic(i_position, i_velocity);
    setState(state);

    // Initialize SFML Entity
    entity = sf::CircleShape(10.f);
    entity.setPosition(i_position[0], i_position[1]);
    entity.setFillColor(color);

    // Initialize window height and width
    setWindow(width, height);

    // Initialize Neighboring Entities
    left_entity = left;
    right_entity = right;
    parent_entity = parent;
}

// Tree Character Constructor
TreeCharacter::TreeCharacter(int nodes, float init_zone[2], sf::Color color, float width, float height) {
    // Save Parameters as Class Variables
    entity_color = color;

    window_height = height;
    window_width = width;

    n_characters = nodes;

    // Call initialization function
    root = initialize(root, 0, init_zone[0], init_zone[1]);
}

// Tree Character Initialization
TreeEntity* TreeCharacter::initialize(TreeEntity *character, int i, float width, float height) {
    // Base Case for Recursion
    if (i < n_characters) {
        float i_position[2];
        i_position[0] = width;
        i_position[1] = height;

        TreeEntity *i_character = new TreeEntity(i_position, entity_color, NULL, NULL, NULL, window_width, window_height);
        character = i_character;

        // Insert Left and Right Children
        character -> left_entity = initialize(character -> left_entity, 2 * i + 1, width - 30, height - 20);
        character -> right_entity = initialize(character -> right_entity, 2 * i + 2, width - 20, height - 30);

        if (character -> left_entity != NULL)
            character -> left_entity -> parent_entity = character;
        if (character -> right_entity != NULL)
            character -> right_entity -> parent_entity = character;
    }

    return character;
}

void TreeCharacter::updateTreeState(Steering steering, std::function<Steering(Kinematic, Kinematic)> ai_algorithm, std::function<Steering(Kinematic, Kinematic)> evade_algorithm, float time) {
    // Update the root
    root -> updateState(steering, time);

    // Recursively update the whole tree
    recursive_update(root -> left_entity, ai_algorithm, evade_algorithm, time);
    recursive_update(root -> right_entity, ai_algorithm, evade_algorithm, time);
}

void TreeCharacter::recursive_update(TreeEntity *character, std::function<Steering(Kinematic, Kinematic)> ai_algorithm, std::function<Steering(Kinematic, Kinematic)> evade_algorithm, float time) {
    // Base Case
    if (character == NULL)
        return;

    // Update the state
    TreeEntity *target_character = character -> parent_entity;
    Steering steering_command = ai_algorithm(target_character -> getState(), character -> getState());

    Steering cohesion_command;
    cohesion_command.linear[0] = 0; cohesion_command.linear[1] = 0;

    if (character -> parent_entity != NULL) {
        if (character -> parent_entity -> left_entity == character) {
            if (character -> parent_entity -> right_entity != NULL)
                cohesion_command = evade_algorithm(character -> parent_entity -> right_entity -> getState(), character -> getState());
        }
        else if (character -> parent_entity -> right_entity == character) {
            if (character -> parent_entity -> left_entity != NULL)
                cohesion_command = evade_algorithm(character -> parent_entity -> left_entity -> getState(), character -> getState());
        }
    }

    steering_command.linear[0] += cohesion_command.linear[0];
    steering_command.linear[1] += cohesion_command.linear[1];
    character -> updateState(steering_command, time);
    
    // Recursion
    recursive_update(character -> left_entity, ai_algorithm, evade_algorithm, time);
    recursive_update(character -> right_entity, ai_algorithm, evade_algorithm, time);
}

bool TreeCharacter::collisionCheck(Kinematic character) {
    return recursive_check(root, character);
}

bool TreeCharacter::recursive_check(TreeEntity *character, Kinematic collider) {
    // Base Case
    if (character == NULL)
        return false;

    // Collision Check for current
    Kinematic character_state = character -> getState();
    float *character_position = character_state.getPosition();
    float *collider_position = collider.getPosition();

    if (fabs(collider_position[0] - character_position[0]) < 10.0 && fabs(collider_position[1] - character_position[1]) < 10.0) {
        return true;
    }

    // Collision Check for neighbors
    bool left_collision = recursive_check(character -> left_entity, collider);
    bool right_collision = recursive_check(character -> right_entity, collider);

    return left_collision || right_collision;
}

void TreeCharacter::render(sf::RenderWindow &window) {
    recursive_render(root, window);
}

void TreeCharacter::recursive_render(TreeEntity *character, sf::RenderWindow &window) {
    // Base Case
    if (character == NULL)
        return;

    // Draw
    window.draw(character -> entity);
    
    // Recursion
    recursive_render(character -> left_entity, window);
    recursive_render(character -> right_entity, window);
}