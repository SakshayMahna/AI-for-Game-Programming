#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>
#include <iostream>
#include <cmath>
#include "selection.h"
#include "character.h"
#include "kinematic.h"
#include "ai.h"

void player_selection(sf::RenderWindow& window, char& player)
{
    // get the size of the window
    sf::Vector2u size = window.getSize();
    float width = size.x;
    float height = size.y;

    // Title Text
    sf::Font font;
    font.loadFromFile("font.ttf");

    ///////////////// Selecting Cat or Rat
    sf::Text title;
    title.setPosition((width / 2) - 130, height / 8);
    title.setCharacterSize(50);
    title.setFillColor(sf::Color::Black);
    title.setFont(font);
    title.setString("Chase Game");
    
    // Two Selection Boxes: Rat and Cat
    sf::RectangleShape rat_box = sf::RectangleShape();
    rat_box.setPosition((width / 4) - 130, (height / 2) - 20);
    rat_box.setSize(sf::Vector2f((width / 4) + 100, height / 8));
    rat_box.setOutlineThickness(1.0);
    rat_box.setOutlineColor(sf::Color::Black);
    sf::Text rat_title;
    rat_title.setPosition((width / 4) - 100, height / 2);
    rat_title.setCharacterSize(30);
    rat_title.setFillColor(sf::Color::Red);
    rat_title.setFont(font);
    rat_title.setString("Play as Rat");

    sf::RectangleShape cat_box = sf::RectangleShape();
    cat_box.setPosition((3 * width / 4) - 130, (height / 2) - 20);
    cat_box.setSize(sf::Vector2f((width / 4) + 100, height / 8));
    cat_box.setOutlineThickness(1.0);
    cat_box.setOutlineColor(sf::Color::Black);
    sf::Text cat_title;
    cat_title.setPosition((3 * width / 4) - 100, height / 2);
    cat_title.setCharacterSize(30);
    cat_title.setFillColor(sf::Color::Red);
    cat_title.setFont(font);
    cat_title.setString("Play as Cat");

    // Run the program as long as the window is open
    while (window.isOpen())
    {
        // Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            
            if (mouse_pos.x > (width / 4) - 130 && mouse_pos.x < (width / 2) -30
                && mouse_pos.y > (height / 2) - 20 && mouse_pos.y < (5 * height / 8) - 20)
                player = 'r';
            
            if (mouse_pos.x > (3 * width / 4) - 130 && mouse_pos.x < (width) -30
                && mouse_pos.y > (height / 2) - 20 && mouse_pos.y < (5 * height / 8) - 20)
                player = 'c';
        }

        // Go to next screen if selection is done
        if (player == 'c' || player == 'r')
            break;

        // Reset
        window.clear(sf::Color::White);

        window.draw(title);
        window.draw(rat_box);
        window.draw(cat_box);
        window.draw(rat_title);
        window.draw(cat_title);

        // End current frame
        window.display();
    }
}

void difficulty_selection(sf::RenderWindow& window, char& difficulty)
{
    // get the size of the window
    sf::Vector2u size = window.getSize();
    float width = size.x;
    float height = size.y;

    // Title Text
    sf::Font font;
    font.loadFromFile("font.ttf");

    ////////////////////// Selecting Difficulty
    sf::Text difficulty_title;
    difficulty_title.setPosition((width / 2) - 180, height / 8);
    difficulty_title.setCharacterSize(50);
    difficulty_title.setFillColor(sf::Color::Black);
    difficulty_title.setFont(font);
    difficulty_title.setString("Select Difficulty");

    // Three Selection Boxes: Easy, Medium and Hard
    sf::RectangleShape easy_box = sf::RectangleShape();
    easy_box.setPosition((2 * width / 10) - 130, (height / 2) - 20);
    easy_box.setSize(sf::Vector2f((width / 10) + 100, height / 8));
    easy_box.setOutlineThickness(1.0);
    easy_box.setOutlineColor(sf::Color::Black);
    sf::Text easy_title;
    easy_title.setPosition((2 * width / 10) - 100, height / 2);
    easy_title.setCharacterSize(30);
    easy_title.setFillColor(sf::Color::Red);
    easy_title.setFont(font);
    easy_title.setString("Easy");

    sf::RectangleShape medium_box = sf::RectangleShape();
    medium_box.setPosition((5 * width / 10) - 130, (height / 2) - 20);
    medium_box.setSize(sf::Vector2f((width / 10) + 100, height / 8));
    medium_box.setOutlineThickness(1.0);
    medium_box.setOutlineColor(sf::Color::Black);
    sf::Text medium_title;
    medium_title.setPosition((5 * width / 10) - 100, height / 2);
    medium_title.setCharacterSize(30);
    medium_title.setFillColor(sf::Color::Red);
    medium_title.setFont(font);
    medium_title.setString("Medium");

    sf::RectangleShape hard_box = sf::RectangleShape();
    hard_box.setPosition((8 * width / 10) - 130, (height / 2) - 20);
    hard_box.setSize(sf::Vector2f((width / 10) + 100, height / 8));
    hard_box.setOutlineThickness(1.0);
    hard_box.setOutlineColor(sf::Color::Black);
    sf::Text hard_title;
    hard_title.setPosition((8 * width / 10) - 100, height / 2);
    hard_title.setCharacterSize(30);
    hard_title.setFillColor(sf::Color::Red);
    hard_title.setFont(font);
    hard_title.setString("Hard");

    // Run the program as long as the window is open
    while (window.isOpen())
    {
        // Check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            
            if (mouse_pos.x > (2 * width / 10) - 130 && mouse_pos.x < (3 * width / 10) -30
                && mouse_pos.y > (height / 2) - 20 && mouse_pos.y < (5 * height / 8) - 20)
                difficulty = 'e';
            
            if (mouse_pos.x > (5 * width / 10) - 130 && mouse_pos.x < (6 * width / 10) -30
                && mouse_pos.y > (height / 2) - 20 && mouse_pos.y < (5 * height / 8) - 20)
                difficulty = 'm';

            if (mouse_pos.x > (8 * width / 10) - 130 && mouse_pos.x < (9 * width / 10) -30
                && mouse_pos.y > (height / 2) - 20 && mouse_pos.y < (5 * height / 8) - 20)
                difficulty = 'h';
        }

        // Go to next screen if selection is done
        if (difficulty == 'e' || difficulty == 'm' || difficulty == 'h')
            break;

        // Reset
        window.clear(sf::Color::White);

        window.draw(difficulty_title);
        window.draw(easy_box);
        window.draw(medium_box);
        window.draw(hard_box);
        window.draw(easy_title);
        window.draw(medium_title);
        window.draw(hard_title);

        // End current frame
        window.display();
    }
}