#include "Game.h"

Game::Game()
    : window(sf::VideoMode({800, 600}), "SFML 3 Game")
{
    window.setFramerateLimit(60);
}

void Game::run()
{
    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            handleEvent(*event);
        }

        float dt = clock.restart().asSeconds();
        update(dt);

        window.clear(sf::Color::White);
        draw();
        window.display();
    }
}

void Game::handleEvent(const sf::Event& event)
{
    if (event.getIf<sf::Event::MouseButtonPressed>() &&
        state == Stand)
    {
        state = Attack;
        player.setAttack();
    }
}

void Game::update(float dt)
{
    player.update(dt);

    if (state == Attack && player.isAttackFinished())
    {
        state = Stand;
        player.setStand();
    }
}

void Game::draw()
{
    player.draw(window);
}
