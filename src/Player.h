#pragma once
#include "Obstacle.h"
class Player :
    public Entity
{
public:
    Player(sf::FloatRect&& bounds, sf::FloatRect&& hitbox_bounds, sf::String texture_path, sf::Vector2f&& max_velocity,
        float acceleration, float deceleration, sf::String nick, sf::Font& font);
    ~Player();

    void move(const float&& dir_x, const float&& dir_y, const float& dt);
    void jump(const float& dt);
    void update(const float& dt, const sf::Vector2i mouse_pos);
    void render(sf::RenderTarget& target);
    void onPlatform(bool x);
    void correctBullets(std::vector<sf::Vector2f>to_stay);
    const bool& rightDir();
    const bool& falling();
    const float& getWeaponRotation();
    const std::vector<sf::Vector2f> getBullets();
private:
    sf::Texture texture2;
    MovementComponent movementComponent;
    Weapon weapon;
    bool rightDirection;
    sf::Text nick;
    //std::unique_ptr<Animation>animation;
};
