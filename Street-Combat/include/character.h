#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "animation.h"

class Character
{
private:
    float health;
    sf::RectangleShape health_entity;

    float position[2];
    sf::Sprite entity;
    TextureManager texture;
    AnimationHandler handler;

public:
    bool block;
    
    Character(float i_position[2], sf::Color color, std::string texture_file, 
              bool enemy, float h_height);
    Character() = default;

    void updatePosition(float d_x, float d_y);
    void animate(int animation_id);
    void render(sf::RenderWindow &window);

    void changeHealth(float dh);

    void initializeAnimations(int left, int top, int width, int height, std::vector<int> durations);
    float *getPosition() { return position; };
    float getHealth() { return health; };
};

#endif