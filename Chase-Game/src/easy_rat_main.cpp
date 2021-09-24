#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>
#include <iostream>
#include <cmath>
#include "character.h"
#include "kinematic.h"
#include "ai.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Easy Chase Game (Rat)");

    // get the size of the window
    sf::Vector2u size = window.getSize();
    float width = size.x;
    float height = size.y;

    // Player
    Character player;
    initialize_player(width, height, player);

    // Enemies
    Character enemies[8];
    initialize_enemies(width, height, enemies);

    // Get starting timepoint (For Score)
    sf::Font font;
    font.loadFromFile("font.ttf");

    sf::Text score;
    score.setPosition(width - 100, height - 50);
    score.setCharacterSize(30);
    score.setFillColor(sf::Color::Black);
    score.setFont(font);

    auto start_time = std::chrono::high_resolution_clock::now();

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
            // right key is pressed
            player.updatePosition(0.1, 0.0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            // left key is pressed
            player.updatePosition(-0.1, 0.0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            // up key is pressed
            player.updatePosition(0.0, -0.1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            // down key is pressed
            player.updatePosition(0.0, 0.1);
        }

        // Check if Rat is caught
        for (int e = 0; e < 8 ; e += 1) {
            Kinematic enemy_state = enemies[e].getState();
            float* enemy_position = enemy_state.getPosition();

            Kinematic player_state = player.getState();
            float* player_position = player_state.getPosition();

            if (fabs(player_position[0] - enemy_position[0]) < 5.0 && fabs(player_position[1] - enemy_position[1]) < 5.0) {
                window.close();
            }
        }

        // AI
        Steering enemy_steering;
        for (int e = 0; e < 8; e += 1){
            enemy_steering = kinematic_seek(player.getState(), enemies[e].getState());
            enemies[e].updateState(enemy_steering, 0.01);
        }

        // Reset
        window.clear(sf::Color::White);

        // Draw
        window.draw(player.entity);
        for (int e = 0; e < 8; e += 1){
            window.draw(enemies[e].entity);
        }

        // Calculate Score based on Time
        auto current_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time);
        score.setString(std::to_string(duration.count()));
        window.draw(score);

        // End current frame
        window.display();
    }

    return 0;
}