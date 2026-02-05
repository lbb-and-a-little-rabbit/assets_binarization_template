#pragma once
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Player.h"

class Game {
public:
    Game();
    void run();

private:
    void handleEvent(const sf::Event& event);
    void update(float dt);
    void draw();

    sf::RenderWindow window;
    sf::Clock clock;
    GameState state = Stand;
    Player player;
};
