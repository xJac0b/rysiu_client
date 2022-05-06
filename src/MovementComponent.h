#pragma once 
#include "Gui.h"
enum movement_states {IDLE = 0, MOVING, MOVING_LEFT, MOVING_RIGHT, MOVING_UP, MOVING_DOWN};

class MovementComponent
{
public:
    MovementComponent(std::vector<sf::Vertex>& sprite, float maxVelocity, float acceleration, float deceleration,
        sf::Vector2f hitboxOffset);
    virtual ~MovementComponent();

    //Accessors
    const float& getMaxVelocity() const;
    const sf::Vector2f& getVelocity() const;

    void move(const float dir_x, const float dir_y, const float & dt);
    void jump();
    void update(const float &dt);
    void setVelocityVector(const float& x, const    float& y);
    void setJumping(const bool& x);
    void setOnPlatform(const bool& x);
    const bool& canJump();
private:
    std::vector<sf::Vertex>& sprite;

    float maxVelocity;
    float acceleration;
    float deceleration;
    bool jumping, up,standingOnPlatform;
    sf::Vector2f velocity;
    sf::Vector2f hitboxOffset;
};

