#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class Player {
public:
    Player();

    void setStand();
    void setAttack();

    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isAttackFinished() const;

private:
    void loadAssets();

private:
    // 注意：texture 必须在 sprite 之前声明（构造顺序）
    sf::Texture dummyTexture;
    sf::Sprite  sprite;

    std::vector<sf::Texture> standFrames;
    std::vector<sf::Texture> attackFrames;

    const std::vector<sf::Texture>* currentFrames = nullptr;

    int currentFrame = 0;
    float timer = 0.f;
    float frameDuration = 0.08f; // 每帧时长
};
