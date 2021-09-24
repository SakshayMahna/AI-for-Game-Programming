#ifndef STAGE_H
#define STAGE_H

#include <SFML/Graphics.hpp>
#include <functional>
#include "character.h"

extern float CATCH_OFFSET;

// Rat Stages
void rat_stage(sf::RenderWindow& window, std::function<void(Character, Character*)> stage);
void easy_rat_stage(Character player, Character enemies[8]);
void medium_rat_stage(Character player, Character enemies[8]);
void hard_rat_stage(Character player, Character enemies[8]);

// Cat Stages
void cat_stage(sf::RenderWindow& window, std::function<void(Character, Character*)> stage);
void easy_cat_stage(Character player, Character enemies[8]);
void medium_cat_stage(Character player, Character enemies[8]);
void hard_cat_stage(Character player, Character enemies[8]);

#endif