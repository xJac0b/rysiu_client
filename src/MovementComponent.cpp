#include "pch.h"
#include "MovementComponent.h"

MovementComponent::MovementComponent(std::vector<sf::Vertex>&sprite,
    float maxVelocity, float acceleration, float deceleration,
    sf::Vector2f hitboxOffset)
    : sprite(sprite), maxVelocity(maxVelocity), acceleration(acceleration), deceleration(deceleration),
    hitboxOffset(hitboxOffset), jumping(false), up(false)
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


const sf::Vector2f & MovementComponent::getVelocity() const
{
    return this->velocity;
}


void MovementComponent::move(const float dir_x, const float dir_y, const float & dt)
{
//Acceleration
this->velocity.x += this->acceleration * dir_x;

this->velocity.y += this->acceleration * dir_y;
}

void MovementComponent::jump()
{
    this->jumping = true;
    this->up = true;
    this->velocity.y = -this->maxVelocity*4;
}

void MovementComponent::update(const float& dt)
{
/*Decelerates the sprite and controls the maximum velocity.
  Moves the sprite.
*/
if(this->velocity.x > 0.f) //Check for positive x
{
        //Max velocity check
    if(this->velocity.x > this->maxVelocity)
        this->velocity.x = this->maxVelocity;

        //Deceleration
    this->velocity.x -= deceleration;
    if(this->velocity.x < 0.f)
        this->velocity.x = 0.f;
}

else if(this->velocity.x < 0.f) //Check for negative x
{
        //Max velocity
    if(this->velocity.x < -this->maxVelocity)
        this->velocity.x = -this->maxVelocity;

        //Deceleration
    this->velocity.x += deceleration;
    if(this->velocity.x > 0.f)
        this->velocity.x = 0.f;
}


//GRAVITY
if (this->jumping && this->up)
{
    this->velocity.y += this->deceleration;
    if (this->velocity.y >= 0)
    {
        this->up = false;
    }
}
else if (!this->up)
{
    if (this->sprite[3].position.y < 600.f /*TODO: but is not standing on a platform*/)
    {
        this->velocity.y += this->acceleration;
    }
    else
    {
        this->velocity.y = 0.f;
        this->jumping = false;
    }
}

   
//Final Move
for (auto& i : this->sprite)
{
    i.position.x += this->velocity.x*dt; //Uses velocity
    i.position.y += this->velocity.y * dt;
}
sf::FloatRect bnds = gui::getVertexShape(this->sprite);
if (bnds.left < -this->hitboxOffset.x)
    gui::setVertexShape(this->sprite, { -this->hitboxOffset.x, bnds.top, bnds.width, bnds.height });
else if (bnds.width + bnds.left > 800 + this->hitboxOffset.x)
gui::setVertexShape(this->sprite, { 800 - bnds.width + this->hitboxOffset.x, bnds.top, bnds.width, bnds.height });

}

const bool& MovementComponent::canJump()
{
    return !this->jumping;
}
