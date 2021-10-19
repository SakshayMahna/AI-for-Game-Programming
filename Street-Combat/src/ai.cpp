#include "ai.h"
#include "character.h"
#include <vector>
#include <cmath>
#include <iostream>

const float ENEMY_OFFSET = 240;
const float ENEMY_SCREEN_OFFSET = 1200 - 20;
const float ACTION_INERTIA = 300;

EnemyAI::EnemyAI(Character &i_player, Character &i_enemy) {
    player = &i_player;
    enemy = &i_enemy;
    action_inertia = ACTION_INERTIA;
}

std::pair<float, float> EnemyAI::player_health_membership() {
    float health = player->getHealth();
    float offensive_points, defensive_points;

    // Assign offensive points
    if (health < 20)
        offensive_points = 1;
    else if (health < 40)
        offensive_points = (40 - health) * 0.05;
    else if (health > 80)
        offensive_points = 1;
    else if (health > 60)
        offensive_points = (health - 60) * 0.05;
    else
        offensive_points = 0;

    // Assign defensive points
    if (health < 20)
        defensive_points = 0;
    else if (health < 40)
        defensive_points = (health - 20) * 0.05;
    else if (health < 60)
        defensive_points = 1;
    else if (health < 80)
        defensive_points = (80 - health) * 0.05;
    else
        defensive_points = 0;

    std::pair<float, float> result = {offensive_points, defensive_points};
    return result;
}

std::pair<float, float> EnemyAI::enemy_health_membership() {
    float health = enemy->getHealth();
    float offensive_points, defensive_points;

    // Assign offensive points
    if (health < 25)
        offensive_points = 0;
    else if (health < 75)
        offensive_points = (health - 25) * 0.02;
    else
        offensive_points = 1;

    // Assign defensive points
    if (health < 25)
        defensive_points = 1;
    else if (health < 75)
        defensive_points = (75 - health) * 0.02;
    else
        defensive_points = 0;

    std::pair<float, float> result = {offensive_points, defensive_points};
    return result;
}

void EnemyAI::offensive_play() {
    float *enemy_pos = enemy->getPosition();
    float *player_pos = player->getPosition();

    if (fabs(enemy_pos[0] - player_pos[0]) > ENEMY_OFFSET) {
        enemy->block = false;
        enemy->updatePosition(-0.1, 0.0);
        enemy->animate(0);
    }
    else {
        enemy->block = false;
        if (fabs(enemy_pos[0] - player_pos[0]) <= ENEMY_OFFSET &&
            player->block == false)
            player->changeHealth(-0.01);

        enemy->animate(1);
    }
}

void EnemyAI::defensive_play() {
    float *enemy_pos = enemy->getPosition();
    float *player_pos = player->getPosition();

    if (fabs(enemy_pos[0]) < ENEMY_SCREEN_OFFSET) {
        enemy->block = false;
        enemy->updatePosition(0.1, 0.0);
        enemy->animate(0);
    }
    else {
        enemy->block = true;
        enemy->animate(2);
    }
}

void EnemyAI::take_action() {
    if (action_inertia == ACTION_INERTIA) {
        std::pair<float, float> fuzzy1 = player_health_membership();
        std::pair<float, float> fuzzy2 = enemy_health_membership();
        
        std::pair<float, float> fuzzy;
        fuzzy.first = 0.25 * fuzzy1.first + 0.75 * fuzzy2.first;
        fuzzy.second = 0.25 * fuzzy1.second + 0.75 * fuzzy2.second;

        // std::cout << fuzzy2.first << " " << fuzzy2.second << std::endl;

        float attack_probability = fuzzy.first / (fuzzy.first + fuzzy.second);
        float probability = ((double) rand() / (RAND_MAX));

        if (probability < attack_probability) {
            offensive_play();
            prev_action = 'o';
        }
        else {
            defensive_play();
            prev_action = 'd';
        }

        action_inertia = 0;
    } else {
        if (prev_action == 'o')
            offensive_play();
        else
            defensive_play();
            
        action_inertia += 1;
    }
}