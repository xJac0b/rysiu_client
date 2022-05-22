#include "pch.h"
#include "MovementComponent.h"
using namespace std;
MovementComponent::MovementComponent(std::vector<sf::Vertex>& sprite,
    float maxVelocity, float acceleration, float deceleration,
    sf::Vector2f hitboxOffset)
    : sprite(sprite), maxVelocity(maxVelocity), acceleration({ 0,3000 }), deceleration(deceleration),
    hitboxOffset(hitboxOffset), jumping(false), up(false), platform(false), velocity({0,0})
{
    this->maxVelocity = maxVelocity;
}

MovementComponent::~MovementComponent()
{

}

const float& MovementComponent::getMaxVelocity() const
{
    return this->maxVelocity;
}


const sf::Vector2f& MovementComponent::getVelocity() const
{
    return this->velocity;
}

void MovementComponent::jump(const float&dt)
{
    this->velocity.y = -960.f;
    this->jumping = true;
}

void MovementComponent::move(const float dir_x, const float dir_y, const float& dt)
{
    //Acceleration
    this->acceleration.x = 1650;
    this->velocity.x += (this->acceleration.x * dir_x * dt);
    if(dir_x < 0)
        this->acceleration.x = -this->acceleration.x;
}

void MovementComponent::update(const float& dt)
{
    if (this->velocity.x > 0.f) //Check for positive x
    {
        //Max velocity check
        if (this->velocity.x > this->maxVelocity)
            this->velocity.x = this->maxVelocity;

        //Deceleration
        this->velocity.x -= deceleration * dt;
        if (this->velocity.x < 0.f)
            this->velocity.x = 0.f;
    }

    else if (this->velocity.x < 0.f) //Check for negative x
    {
        //Max velocity
        if (this->velocity.x < -this->maxVelocity)
            this->velocity.x = -this->maxVelocity;

        //Deceleration
        this->velocity.x += deceleration * dt;
        if (this->velocity.x > 0.f)
            this->velocity.x = 0.f;
    }

    if (this->sprite[3].position.y < 600.f && !this->platform)
    {
        this->velocity.y += acceleration.y * dt;
    }  
    else
    {
        this->jumping = false;
        if (this->velocity.y > 0.f)
            this->velocity.y = 0.f;
    }
       
    //cout << this->platform;
    //Final Move
    for (auto& i : this->sprite)
    {
       i.position.x += (velocity.x * dt) + (acceleration.x * dt * dt / 2.f);
      // if (!this->platform)
       i.position.y += (velocity.y * dt);
       //else
          // i.position.y += (velocity.y * dt);
    }
   // if (this->sprite[0].position.y < 300)
        //cout << this->platform << endl;//cout << this->sprite[0].position.y << endl;
    acceleration = { 0.f,acceleration.y };


    sf::FloatRect bnds = gui::getVertexShape(this->sprite);
    if (bnds.left < -this->hitboxOffset.x)
        bnds.left = -this->hitboxOffset.x;
    else if (bnds.width + bnds.left > 800 + this->hitboxOffset.x)
        bnds.left = 800 - bnds.width + this->hitboxOffset.x;
    if (bnds.top < -this->hitboxOffset.y)
    {
        bnds.top = -this->hitboxOffset.y;
        this->up = false;
        this->velocity.y = 0;
    }
    else if (bnds.top+bnds.height >= 600)
    {
        bnds.top = 600 - bnds.height;
    }
    gui::setVertexShape(this->sprite, { bnds.left, bnds.top, bnds.width, bnds.height });
}

const bool& MovementComponent::canJump()
{
    return !this->jumping;
}

void MovementComponent::onPlatform(bool x)
{
    this->platform = x;
    if (x)
    {
        this->jumping = false;
        this->velocity.y = 0.f;
    }
}

const bool& MovementComponent::falling()
{
    if (this->velocity.y >= 0.f)
        return true;
    return false;
}
