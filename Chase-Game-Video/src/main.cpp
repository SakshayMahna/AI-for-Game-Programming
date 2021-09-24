#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>
#include <iostream>
#include <cmath>
#include "character.h"
#include "kinematic.h"
#include "ai.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Chase Game");

    // Get size of window
    sf::Vector2u size = window.getSize();
    float width = size.x;
    float height = size.y;

    // Player
    Character player;
    float player_position[2] = {width / 2, height / 2};
    player = Character(player_position, sf::Color::Green, width, height);

    // Enemies
    Character enemies[8];
    float enemy_position[8][2];

    enemy_position[0][0] = width / 4;
    enemy_position[0][1] = height / 2;
    enemies[0] = Character(enemy_position[0], sf::Color::Red, width, height);

    enemy_position[1][0] = width / 4;
    enemy_position[1][1] = height / 4;
    enemies[1] = Character(enemy_position[1], sf::Color::Red, width, height);

    enemy_position[2][0] = width / 2;
    enemy_position[2][1] = height / 4;
    enemies[2] = Character(enemy_position[2], sf::Color::Red, width, height);

    enemy_position[3][0] = 3 * width / 4;
    enemy_position[3][1] = height / 4;
    enemies[3] = Character(enemy_position[3], sf::Color::Red, width, height);
    
    enemy_position[4][0] = 3 * width / 4;
    enemy_position[4][1] = height / 2;
    enemies[4] = Character(enemy_position[4], sf::Color::Red, width, height);

    enemy_position[5][0] = 3 * width / 4;
    enemy_position[5][1] = 3 * height / 4;
    enemies[5] = Character(enemy_position[5], sf::Color::Red, width, height);

    enemy_position[6][0] = width / 2;
    enemy_position[6][1] = 3 * height / 4;
    enemies[6] = Character(enemy_position[6], sf::Color::Red, width, height);

    enemy_position[7][0] = width / 4;
    enemy_position[7][1] = 3 * height / 4;
    enemies[7] = Character(enemy_position[7], sf::Color::Red, width, height);

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

        // Check if Rat is caught
        for (int e = 0; e < 8; e += 1) {
            Kinematic enemy_state = enemies[e].getState();
            float *enemy_position = enemy_state.getPosition();

            Kinematic player_state = player.getState();
            float *player_position = player_state.getPosition();

            if (fabs(player_position[0] - enemy_position[0]) < 5.0 && fabs(player_position[1] - enemy_position[1]) < 5.0) {
                window.close();
            }
        }

        // AI
        Steering enemy_steering;
        for (int e = 0; e < 4; e += 1) {
            enemy_steering = kinematic_seek(player.getState(), enemies[e].getState());
            enemies[e].updateState(enemy_steering, 0.01);
        }
        for (int e = 4; e < 8; e += 1) {
            enemy_steering = kinematic_pursue(player.getState(), enemies[e].getState());
            enemies[e].updateState(enemy_steering, 0.01);
        }

        // Reset
        window.clear(sf::Color::White);

        // Draw
        window.draw(player.entity);
        for (int e = 0; e < 8; e += 1)
            window.draw(enemies[e].entity);

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