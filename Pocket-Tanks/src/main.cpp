#include <SFML/Graphics.hpp>
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include "character.h"
#include "kinematic.h"
#include "ai.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "Pocket Tanks");

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
    float c1_position[2] = {width/8, 3 * height/4 - 10};
    Character c1 = Character(c1_position, 0, sf::Color::Black);

    float c2_position[2] = {7 * width/8, 3 * height/4 - 10};
    Character c2 = Character(c2_position, 0, sf::Color::Red);

    // Aimer and Projectile for Players
    MouseAim c1_aimer = MouseAim(c1_position, 0, sf::Color::Black);
    Projectile *c1_projectile, *c2_projectile;
    bool projectile_shot = false, player_turn = true;

    // Score for players
    sf::Font font;
    font.loadFromFile("font.ttf");

    int c1_points = 0, c2_points = 0;
    sf::Text c1_score, c2_score;
    c1_score.setPosition(100, height - 80);
    c1_score.setCharacterSize(50);
    c1_score.setFillColor(sf::Color::Black);
    c1_score.setFont(font);
    c2_score.setPosition(width - 100, height - 80);
    c2_score.setCharacterSize(50);
    c2_score.setFillColor(sf::Color::Red);
    c2_score.setFont(font);

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

        // Player turn
        if (player_turn) {
            // Move Aimer
            sf::Vector2i aim_position = sf::Mouse::getPosition(window);
            c1_aimer.updatePosition(aim_position.x, aim_position.y);

            // Detect shoot
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !projectile_shot) {
                Steering c1_velocity = manual_aim(c1_aimer.getState(), c1.getState());
                // Steering c1_velocity = calculated_aim(c2.getState(), c1.getState());
                c1_projectile = new Projectile(c1_position, c1_velocity.linear, sf::Color::Black);
                projectile_shot = true;
            }

            // Reset
            window.clear(sf::Color::White);

            if (projectile_shot) {
                c1_projectile->update(0.005, 0);
                window.draw(c1_projectile->entity);

                // Update score if enemy is shot
                if (c1_projectile->getHeight() > 3 * height / 4) {
                    projectile_shot = false;
                    player_turn = false;

                    if (fabs(c1_projectile->getRange() - 7*width/8) < 30) {
                        c1_points += 1;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    delete c1_projectile;
                }
            }

            // Draw
            window.draw(c1.entity);
            window.draw(c2.entity);
            window.draw(ground);
            window.draw(c1_aimer.entity);

            c1_score.setString(std::to_string(c1_points));
            c2_score.setString(std::to_string(c2_points));
            window.draw(c1_score);
            window.draw(c2_score);          

            // End current frame
            window.display();
        }
        // Opponent Turn
        else {
            // Detect shoot
            if (!projectile_shot) {
                Steering c2_velocity = calculated_aim(c1.getState(), c2.getState());
                c2_projectile = new Projectile(c2_position, c2_velocity.linear, sf::Color::Red);
                projectile_shot = true;
            }

            // Reset
            window.clear(sf::Color::White);

            if (projectile_shot) {
                c2_projectile->update(0.005, 0);

                window.draw(c2_projectile->entity);

                // Update score if enemy is shot
                if (c2_projectile->getHeight() > 3 * height / 4) {
                    projectile_shot = false;
                    player_turn = true;

                    if (fabs(c2_projectile->getRange() - width/8) < 30) {
                        c2_points += 1;
                    }

                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    delete c2_projectile;
                }
            }

            // Draw
            window.draw(c1.entity);
            window.draw(c2.entity);
            window.draw(ground);

            c1_score.setString(std::to_string(c1_points));
            c2_score.setString(std::to_string(c2_points));   
            window.draw(c1_score);
            window.draw(c2_score);         

            // End current frame
            window.display();
        }
    }

    return 0;
}