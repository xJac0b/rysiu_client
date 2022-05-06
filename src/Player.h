#pragma once
#include "Obstacle.h"
class Player :
    public Entity
{
public:
    Player(sf::FloatRect&& bounds, sf::FloatRect&& hitbox_bounds, sf::String texture_path, sf::Vector2f&& max_velocity,
        float acceleration, float deceleration);
    ~Player();

    void move(const float&& dir_x, const float&& dir_y, const float& dt);
    void jump();
    void fall(const float& dt);
    void update(const float& dt, const sf::Vector2i mouse_pos);
    void render(sf::RenderTarget& target);

    void setVelocityX(const float& x);
    void setVelocityY(const float& y);
    void setJumping(const bool& x);
    void setOnPlatform(const bool& x);

private:
    sf::Texture texture2;
    MovementComponent movementComponent;
    //std::unique_ptr<Animation>animation;
};

