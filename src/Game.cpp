#include "Game.h"

Game::Game()
    : window(sf::VideoMode({800, 600}), "SFML 3 Game")
{
    window.setFramerateLimit(60);

    auto size = window.getSize();

    player.setPosition({
        size.x / 2.f,
        size.y / 2.f
    });
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
    // ===== 攻击 =====
    if (state == Stand)
    {
        if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse->button == sf::Mouse::Button::Left)
            {
                state = Attack;
                player.setAttack();
                return;
            }
        }

        // ===== 行走（按下 A 或 D）=====
        if (const auto* key = event.getIf<sf::Event::KeyPressed>())
        {
            if (key->code == sf::Keyboard::Key::A ||
                key->code == sf::Keyboard::Key::D)
            {
                state = Walk;
                player.setWalk();
                return;
            }
        }
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

    if (state == Walk)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            player.setDirection(1);
            player.move(200.f * dt);
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            player.setDirection(-1);
            player.move(-200.f * dt);
        }
        else
        {
            state = Stand;
            player.setStand();
        }
    }



}

void Game::draw()
{
    player.draw(window);
}
