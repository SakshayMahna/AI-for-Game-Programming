#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include "character.h"
#include "animation.h"
#include "ai.h"

const float SCREEN_OFFSET = 20;
const float PLAYERS_OFFSET = 240;

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "Street Combat");

    // get the size of the window
    sf::Vector2u size = window.getSize();
    float width = size.x;
    float height = size.y;

    // Ground
    sf::RectangleShape ground;
    ground.setSize(sf::Vector2f(width, height));
    ground.setPosition(0, 3 * height / 4);
    ground.setFillColor(sf::Color::Green);

    // Characters
    float player_position[2] = {width/8, 3 * height/4 - 166};
    Character player = Character(player_position, sf::Color::Green, "player_texture_transparent.png", false, 100);
    std::vector<int> player_durations = {6, 4, 2, 2};
    player.initializeAnimations(0, 0, 100, 120, player_durations);

    float enemy_position[2] = {7 * width/8, 3 * height/4 - 166};
    Character enemy = Character(enemy_position, sf::Color::Red, "player_texture_transparent.png", true, 200);
    std::vector<int> enemy_durations = {6, 4, 2, 2};
    enemy.initializeAnimations(0, 0, 100, 120, enemy_durations);

    // Enemy AI
    EnemyAI ai = EnemyAI(player, enemy);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Move
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            // right key pressed
            player.block = false;
            float *enemy_pos = enemy.getPosition();
            float *player_pos = player.getPosition();

            if (fabs(enemy_pos[0] - player_pos[0]) > PLAYERS_OFFSET)
                player.updatePosition(0.1, 0.0);

            player.animate(0);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            // left key pressed
            player.block = false;
            float *player_pos = player.getPosition();

            if (player_pos[0] > SCREEN_OFFSET)
                player.updatePosition(-0.1, 0.0);

            player.animate(0);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            // A key pressed
            player.block = false;
            float *enemy_pos = enemy.getPosition();
            float *player_pos = player.getPosition();

            if (fabs(enemy_pos[0] - player_pos[0]) <= PLAYERS_OFFSET &&
                enemy.block == false)
                enemy.changeHealth(-0.01);

            player.animate(1);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            // A key pressed
            player.block = true;
            player.animate(2);
        }
        else {
            player.block = false;
            player.animate(3);
        }

        // Check health
        if (player.getHealth() <= 0 || enemy.getHealth() <= 0)
            break;

        // enemy.animate(3);
        ai.take_action();
        
        // Clear current frame
        window.clear(sf::Color::White);

        // Draw
        player.render(window);
        enemy.render(window);  
        window.draw(ground);   

        // End current frame
        window.display();
        
    }

    while(true);

    return 0;
}