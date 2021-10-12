#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include "character.h"
#include "ai.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 1100), "Grid Race");

    // get the size of the window
    sf::Vector2u size = window.getSize();
    float width = size.x;
    float height = size.y;

    // Grid
    int rows = 21, columns = 20;
    Grid grid = Grid(rows, columns, 10, 10);
    int end_position[2] = {rows / 2, columns - 1};

    // Characters
    int player_position[2] = {0, 0};
    Character player = Character(player_position, sf::Color::Green);

    int opponent_position[2] = {rows - 1, 0};
    Character opponent = Character(opponent_position, sf::Color::Red);

    // AI
    DijkstraAlgorithm ai = DijkstraAlgorithm(rows, columns, opponent_position, end_position, grid.getObstacles());
    std::vector<pi> shortest_path = ai.plan();

    for (pi p : shortest_path)
        std::cout << p.first << " " << p.second << std::endl;

    // Turn Mechanics
    bool player_turn = true;
    int path_index = 0;

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

        if (player_turn) {
            // Move
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                // right key is pressed
                int* position = player.getPosition();
                int check_position[2] = {position[0], position[1] + 1};
                if (check_position[1] - 1 < columns - 1 &&
                    !grid.check_obstacle(check_position)) {
                    player.updatePosition(0, 1);
                    player_turn = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                // left key is pressed
                int* position = player.getPosition();
                int check_position[2] = {position[0], position[1] - 1};
                if (check_position[1] + 1 > 0 &&
                    !grid.check_obstacle(check_position)) {
                    player.updatePosition(0, -1);
                    player_turn = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                // up key is pressed
                int* position = player.getPosition();
                int check_position[2] = {position[0] - 1, position[1]};
                if (check_position[0] + 1 > 0 &&
                    !grid.check_obstacle(check_position)){
                    player.updatePosition(-1, 0);
                    player_turn = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                // down key is pressed
                int* position = player.getPosition();
                int check_position[2] = {position[0] + 1, position[1]};
                if (check_position[0] - 1 < rows - 1 &&
                    !grid.check_obstacle(check_position)) {
                    player.updatePosition(1, 0);
                    player_turn = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
        else {
            int *now_position = opponent.getPosition();
            std::pair<int, int> next_position = shortest_path[path_index++];

            opponent.updatePosition(next_position.first - now_position[0], 
                                    next_position.second - now_position[1]);

            player_turn = true;

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // Reset
        window.clear(sf::Color::White);

        // Draw
        grid.render(window, player, opponent);

        // End current frame
        window.display();

        // Check if one of the players won
        int *p_position = player.getPosition();
        int *o_position = opponent.getPosition();
        if ((p_position[0] == end_position[0] && p_position[1] == end_position[1]) ||
            (o_position[0] == end_position[0] && o_position[1] == end_position[1]))
            break;
    }

    while(true);

    return 0;
}