#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <functional>
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
    void setState(Kinematic i_state) { state = i_state; }
    void setWindow(float width, float height) { window_width = width; window_height = height; }
};

// TreeEntity Class
class TreeEntity : public Character {
public:
    TreeEntity *left_entity, *right_entity, *parent_entity;

    TreeEntity(float i_position[2], sf::Color color, 
               TreeEntity *left, TreeEntity *right, TreeEntity *parent,
               float width, float height);
    TreeEntity() = default;
};

// TreeCharacter Class
class TreeCharacter {
private:
    float window_width, window_height;
    int n_characters;
    sf::Color entity_color;
    TreeEntity *root;

    TreeEntity *initialize(TreeEntity *character, int i, float width, float height);
    void recursive_update(TreeEntity *character, std::function<Steering(Kinematic, Kinematic)> ai_algorithm, std::function<Steering(Kinematic, Kinematic)> evade_algorithm, float time);
    bool recursive_check(TreeEntity *character, Kinematic collider);
    void recursive_render(TreeEntity *character, sf::RenderWindow &window);

public:
    TreeCharacter(int nodes, float init_zone[4], sf::Color color, float width, float height);
    TreeCharacter() = default;

    void updateTreeState(Steering steering, std::function<Steering(Kinematic, Kinematic)> ai_algorithm, std::function<Steering(Kinematic, Kinematic)> evade_algorithm, float time);
    bool collisionCheck(Kinematic character);
    void render(sf::RenderWindow &window);
    Kinematic getState() { return root -> getState(); }
};

#endif