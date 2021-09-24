#ifndef SELECTION_H
#define SELECTION_H

#include <SFML/Graphics.hpp>

// Select the Player
void player_selection(sf::RenderWindow& window, char& player);

// Select the difficulty
void difficulty_selection(sf::RenderWindow& window, char& difficulty);

#endif