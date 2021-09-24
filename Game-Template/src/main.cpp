#include <SFML/Graphics.hpp>
#include "character.h"
#include "kinematic.h"
#include "seek.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Game Template");

    // get the size of the window
    sf::Vector2u size = window.getSize();
    float width = size.x;
    float height = size.y;

    // Characters
    float c1_position[2] = {width/2, height/2};
    Character c1 = Character(c1_position, 0, sf::Color::Green);

    float c2_position[2] = {width/4, height/2};
    Character c2 = Character(c2_position, 0, sf::Color::Red);

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
            c1.updatePosition(0.1, 0.0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            // left key is pressed
            c1.updatePosition(-0.1, 0.0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            // up key is pressed
            c1.updatePosition(0.0, -0.1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            // down key is pressed
            c1.updatePosition(0.0, 0.1);
        }

        // AI
        Steering c2_steering = kinematic_seek(c1.getState(), c2.getState());
        c2.updateState(c2_steering, 0.01);

        // Reset
        window.clear(sf::Color::White);

        // Draw
        window.draw(c1.entity);
        window.draw(c2.entity);

        // End current frame
        window.display();
    }

    return 0;
}