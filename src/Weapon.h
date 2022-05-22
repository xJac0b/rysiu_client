#pragma once
#include "Enemy.h"

class Bullet
{
public:
    sf::CircleShape shape;
    sf::Vector2f currVelocity;
    float maxSpeed;
    void setPos(sf::Vector2f pos)
    {
        this->shape.setPosition(pos);
    }
    const sf::Vector2f getPos()
    {
        return this->shape.getPosition();
    }
    Bullet(float radius = 4.f, sf::Vector2f pos = { -1,-1 })
        : currVelocity(0.f, 0.f), maxSpeed(1000.f)
    {
        this->shape.setRadius(radius);
        this->shape.setFillColor(sf::Color(150,150,150,255));
        this->shape.setOutlineThickness(1.f);
        this->shape.setOutlineColor(sf::Color::Black);
        if(pos != sf::Vector2f(-1,-1))
            this->shape.setPosition(pos);
    }
};

class Weapon
{
public:
    Weapon(sf::FloatRect bounds, sf::String texture_path1, float bulletVelocity,
        float reload_duration, float shot_duration);
    ~Weapon();
    void setDirection(bool right = true);
    void shot(sf::Vector2i mouse_pos);
    void update(const float& dt, sf::Vector2i mouse_pos, sf::Vector2f pos);
    void render(sf::RenderTarget& target);
    void correctBullets(std::vector<sf::Vector2f>to_stay);
    void reload();
    const std::vector<sf::Vector2f> getBullets();
    const float& getRotation();
private:
    sf::Sprite sprite;
    sf::Texture texture1;
    int ammunition;
    Timer reloadTimer, shotTimer;
    std::vector<Bullet> bullets;
    sf::SoundBuffer bufferShot, bufferReload;
    sf::Sound soundShot, soundReload;
    bool reloading;
};
