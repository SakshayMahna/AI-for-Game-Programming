#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>

class Character
{
private:
    int position[2];
    int previous_position[2];
    sf::Color color;

public:
    Character(int i_position[2], sf::Color i_color);
    Character() = default;

    void updatePosition(int d_x, int d_y);

    int* getPosition() { return position; }
    int* getPreviousPosition() { return previous_position; }
    sf::Color getColor() { return color; }
};

class Grid {
private:
    int rows, columns;
    sf::RectangleShape** grid;
    std::vector<std::pair<int, int>> obstacles;

public:
    Grid(int g_rows, int g_columns, int start_x, int start_y);
    Grid() = default;

    void render(sf::RenderWindow &window, Character c1, Character c2);
    bool check_obstacle(int position[2]);

    std::vector<std::pair<int, int>> getObstacles() { return obstacles; } 
};

#endif