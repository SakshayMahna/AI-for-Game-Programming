#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <string>
#include <iostream>
#include <cmath>
#include "selection.h"
#include "stage.h"
#include "character.h"
#include "kinematic.h"
#include "ai.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "Chase Game");

    char player = 'p', difficulty = 'd';

    player_selection(window, player);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    difficulty_selection(window, difficulty);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    if (player == 'r') {
        if (difficulty == 'e'){
            MAX_SPEED = 0.1;
            CATCH_OFFSET = 5.0;
            rat_stage(window, &easy_rat_stage);
        }
        else if (difficulty == 'm'){
            MAX_SPEED = 0.2;
            CATCH_OFFSET = 10.0;
            rat_stage(window, &medium_rat_stage);
        }
        else if (difficulty == 'h'){
            MAX_SPEED = 0.3;
            CATCH_OFFSET = 15.0;
            rat_stage(window, &hard_rat_stage);
        }
    }
    else if (player == 'c') {
        if (difficulty == 'e') {
            MAX_SPEED = 0.1;
            CATCH_OFFSET = 15.0;
            cat_stage(window, &easy_cat_stage);
        }
        else if (difficulty == 'm'){
            MAX_SPEED = 0.2;
            CATCH_OFFSET = 10.0;
            cat_stage(window, &medium_cat_stage);
        }
        else if (difficulty == 'h') {
            MAX_SPEED = 0.3;
            CATCH_OFFSET = 5.0;
            cat_stage(window, &hard_cat_stage);
        }
    }
    
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
    }
}