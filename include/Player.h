#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Player {
public:
    Player();

    void setStand();
    void setAttack();
    void setWalk();

    void update(float dt);
    void draw(sf::RenderWindow& window);

    void move(float dx);
    void setDirection(int dir);
    void setPosition(const sf::Vector2f& pos);

    bool isAttackFinished() const;

private:
    void loadAssets();
    void applyFrame();   // 统一换帧函数

private:
    sf::Texture dummyTexture;
    sf::Sprite  sprite;

    std::vector<sf::Texture> standFrames;
    std::vector<sf::Texture> attackFrames;
    std::vector<sf::Texture> walkFrames;

    const std::vector<sf::Texture>* currentFrames = nullptr;

    int currentFrame = 0;
    int direction = 1;

    float timer = 0.f;
    float frameDuration = 0.08f;
};
