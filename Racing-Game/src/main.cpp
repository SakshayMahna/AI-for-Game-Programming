#include <SFML/Graphics.hpp>
#include <cstdlib>
#include "character.h"
#include "kinematic.h"
#include "ai.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 1000), "Racing Game");

    // get the size of the window
    sf::Vector2u size = window.getSize();
    float width = size.x;
    float height = size.y;

    // Characters
    float c1_position[2] = {3 * width / 4, height - 50};
    Character c1 = Character(c1_position, -90, sf::Color::Green);

    float c2_position[2] = {width / 4, height - 50};
    Character c2 = Character(c2_position, -90, sf::Color::Red);

    // Target
    float target_position[2] = {width / 2, 0};
    float target_velocity[2] = {0, 0};
    Kinematic target = Kinematic(target_position, target_velocity, 0, 0);

    // Obstacles
    Obstacles obstacles = Obstacles(width, height);

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

        // Move
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            // right key is pressed
            c1.updatePosition(0.0, 0.02);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            // left key is pressed
            c1.updatePosition(0.0, -0.02);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            // up key is pressed
            c1.updatePosition(0.02, 0.0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            // down key is pressed
            c1.updatePosition(-0.02, 0.0);
        }

        // AI
        Steering c2_steering = look_where_going_obstacles(obstacles.getStates(), target, c2.getState());
        c2.updateState(c2_steering, 0.01);

        // Check for Game Over
        Kinematic c1_state = c1.getState();
        float *c1_pos = c1_state.getPosition();
        if (c1_pos[1] < 0)
            break;

        Kinematic c2_state = c2.getState();
        float *c2_pos = c2_state.getPosition();
        if (c2_pos[1] < 0)
            break;

        Kinematic* obstacle_states = obstacles.getStates();
        bool collided = false;
        for (int o = 0; o < 20; o += 1) {
            float *o_pos = obstacle_states[o].getPosition();
            if (c1_pos[0] > o_pos[0] - 15 && c1_pos[0] < o_pos[0] + 15
                && c1_pos[1] > o_pos[1] - 5 && c1_pos[1] < o_pos[1] + 5)
                collided = true;
            if (c2_pos[0] > o_pos[0] - 15 && c2_pos[0] < o_pos[0] + 15
                && c2_pos[1] > o_pos[1] - 5 && c2_pos[1] < o_pos[1] + 5)
                collided = true;
        }

        if (collided)
            break;

        // Reset
        window.clear(sf::Color::White);

        // Draw
        c1.render(window);
        c2.render(window);
        obstacles.render(window);

        // End current frame
        window.display();
    }

    // Keep looping until closed
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
    }

    return 0;
}