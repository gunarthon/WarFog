#include <iostream>
#include <SFML/Graphics.hpp>
#include "ControllerGame.h"
#include "InputManager.h"

// This is the main C++ program- Duh!
// It is where our game starts from
int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    //sf::RenderWindow window(sf::VideoMode::getFullscreenModes()[0], "Gambearra", sf::Style::Fullscreen, settings);
    sf::RenderWindow window(sf::VideoMode(800,800), "Gambearra", sf::Style::Default, settings);
    window.setKeyRepeatEnabled(false);

    ControllerGame *contGame = new ControllerGame(window);
    InputManager *input = new InputManager();
    contGame->Init();
    input->SetController(contGame);

    sf::Font font;
    font.loadFromFile("Carlito-Bold.ttf");

    sf::Clock clock;
    sf::Time time;

    // This "while" loop goes round and round- perhaps forever
    while (window.isOpen())
    {
        time = sf::seconds(0);
        // The next 6 lines of code detect if the window is closed
        // And then shuts down the program
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                // Someone closed the window- bye
                window.close();
            else
                input->Update(event, window);

        }

        time = clock.getElapsedTime();
        if(time > sf::seconds(1.0f/60))
        {
            clock.restart();

            contGame->Update();
            if(contGame->GetExit())
                window.close();

            window.setView(window.getDefaultView());
            contGame->DrawAll();
            window.display();
        }

    }

    delete contGame;
    delete input;

    return 0;
}
