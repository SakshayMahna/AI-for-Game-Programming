#ifndef AI_H
#define AI_H

#include "character.h"

class EnemyAI {
private:
    Character *player;
    Character *enemy;

    int action_inertia;
    char prev_action;

    std::pair<float, float> player_health_membership();
    std::pair<float, float> enemy_health_membership();
    
    void offensive_play();
    void defensive_play();

public:
    EnemyAI(Character &i_player, Character &i_enemy);
    EnemyAI() = default;

    void take_action();
};

#endif