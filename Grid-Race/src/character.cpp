#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include <utility>
#include "character.h"

const float GRID_SIZE = 50.0;  
const float OBSTACLE_PROBABILITY = 0.2;

// Character constructor
Character::Character(int i_position[2], sf::Color i_color) {
    // Initialize Kinematic State
    position[0] = i_position[0];
    position[1] = i_position[1];

    previous_position[0] = i_position[0];
    previous_position[1] = i_position[1];

    color = i_color;
}

// Character Position Update
void Character::updatePosition(int d_x, int d_y) {
    // Update position
    previous_position[0] = position[0];
    previous_position[1] = position[1];

    position[0] += d_x;
    position[1] += d_y;
}

// Grid Constructor
Grid::Grid(int g_rows, int g_columns, int start_x, int start_y) {
    // Initialize the grid
    rows = g_rows;
    columns = g_columns;

    grid = new sf::RectangleShape*[rows];
    for (int i = 0; i < rows; i += 1)
        grid[i] = new sf::RectangleShape[columns];


    float random_value;
    for (int i = 0; i < rows; i += 1) {
        for (int j = 0; j < columns; j += 1) {
            grid[i][j] = sf::RectangleShape(sf::Vector2f(GRID_SIZE, GRID_SIZE));
            grid[i][j].setPosition(start_x + j * GRID_SIZE, start_y + i * GRID_SIZE);
            grid[i][j].setOutlineColor(sf::Color::Black);
            grid[i][j].setOutlineThickness(1.0);

            if ((i != 0 || j != 0) && 
                (i != rows - 1 || j != 0) &&
                (i != rows/2 || j != columns - 1)) {
                random_value = ((double) rand() / (RAND_MAX));
                if (random_value < OBSTACLE_PROBABILITY) {
                    obstacles.push_back(std::make_pair(i, j));
                    grid[i][j].setFillColor(sf::Color::Black);
                }
            }
        }
    }

    grid[rows/2][columns - 1].setFillColor(sf::Color::Yellow);
}

// Render Grid
void Grid::render(sf::RenderWindow &window, Character c1, Character c2) {
    int* c1_position = c1.getPosition();
    int* c1_previous_position = c1.getPreviousPosition();

    int* c2_position = c2.getPosition();
    int* c2_previous_position = c2.getPreviousPosition();

    // Change Colors
    grid[c1_previous_position[0]][c1_previous_position[1]].setFillColor(sf::Color::White);
    grid[c1_position[0]][c1_position[1]].setFillColor(c1.getColor());

    grid[c2_previous_position[0]][c2_previous_position[1]].setFillColor(sf::Color::White);
    grid[c2_position[0]][c2_position[1]].setFillColor(c2.getColor());

    for (int i = 0; i < rows; i += 1) {
        for (int j = 0; j < columns; j += 1) {
            window.draw(grid[i][j]);
        }
    }
}

// Check for obstacle
bool Grid::check_obstacle(int position[2]) {
    for (int i = 0; i < obstacles.size(); i += 1) {
        if (obstacles[i].first == position[0] && obstacles[i].second == position[1])
            return true;
    }

    return false;
}