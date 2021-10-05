#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>
#include <iostream>
#include <cmath>
#include "character.h"
#include "kinematic.h"
#include "ai.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Zombie Escape");

    // Get size of window
    sf::Vector2u size = window.getSize();
    float width = size.x;
    float height = size.y;

    // Player
    Character player;
    float player_position[2] = {width / 2, height / 2};
    player = Character(player_position, sf::Color::Red, width, height);

    // Enemy
    TreeCharacter enemy[3];
    float init_zone_1[2] = {width / 2 - 50, height / 2 - 50};
    enemy[0] = TreeCharacter(50, init_zone_1, sf::Color::Green, width, height);

    float init_zone_2[2] = {width / 2 - 100, height / 2};
    enemy[1] = TreeCharacter(50, init_zone_2, sf::Color::Green, width, height);

    float init_zone_3[2] = {width / 2, height / 2 - 100};
    enemy[2] = TreeCharacter(50, init_zone_3, sf::Color::Green, width, height);

    // Get starting timepoint (for Score)
    sf::Font font;
    font.loadFromFile("font.ttf");

    sf::Text score;
    score.setPosition(width - 100, height - 50);
    score.setCharacterSize(30);
    score.setFillColor(sf::Color::Black);
    score.setFont(font);

    auto start_time = std::chrono::high_resolution_clock::now();

    // Run the program as long as the window is open
    while (window.isOpen()) {
        // Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close requested
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Move
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            // right key pressed
            player.updatePosition(0.1, 0.0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            // left key pressed
            player.updatePosition(-0.1, 0.0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            // up key pressed
            player.updatePosition(0.0, -0.1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            // down key pressed
            player.updatePosition(0.0, 0.1);
        }

        // Check if Player is caught
        bool collision_1 = enemy[0].collisionCheck(player.getState());
        bool collision_2 = enemy[1].collisionCheck(player.getState());
        bool collision_3 = enemy[2].collisionCheck(player.getState());
        if (collision_1 || collision_2 || collision_3)
            window.close();

        // AI
        Steering enemy_steering_1 = kinematic_arrive(player.getState(), enemy[0].getState());
        enemy[0].updateTreeState(enemy_steering_1, kinematic_arrive, kinematic_separation, 0.1);

        Steering enemy_steering_2 = kinematic_arrive(player.getState(), enemy[1].getState());
        enemy[1].updateTreeState(enemy_steering_2, kinematic_arrive, kinematic_separation, 0.1);

        Steering enemy_steering_3 = kinematic_arrive(player.getState(), enemy[2].getState());
        enemy[2].updateTreeState(enemy_steering_3, kinematic_arrive, kinematic_separation, 0.1);

        // Reset
        window.clear(sf::Color::White);

        // Draw
        window.draw(player.entity);
        enemy[0].render(window);
        enemy[1].render(window);
        enemy[2].render(window);

        // Calculate score based on time
        auto current_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
        score.setString(std::to_string(duration.count()));
        window.draw(score);

        // End current frame
        window.display();
    }

    return 0;
}