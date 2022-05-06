#include "pch.h"
#include "Player.h"

Player::Player(sf::FloatRect&& bounds, sf::FloatRect&& hitbox_bounds, sf::String texture_path, sf::Vector2f&& max_velocity,
	float acceleration, float deceleration)
	: Entity(bounds, texture_path, hitbox_bounds),
	movementComponent(sprite, max_velocity.x, acceleration, deceleration, hitboxOffset)
{
	this->texture2.loadFromFile(texture_path.substring(0, texture_path.find(".")) + "1.png");
	gui::setVertexShape(this->sprite, sf::FloatRect(0.f, 0.f,
		200.f, 200.f), 0);
	//this->animation = std::make_unique<Animation>(this->sprite, 2U, sf::Vector2f(256.f, 256.f), 0.5f);
}

Player::~Player()
{
}

void Player::move(const float&& dir_x, const float&& dir_y, const float& dt)
{
	this->movementComponent.move(dir_x, dir_y, dt);
}

void Player::jump()
{
	this->movementComponent.jump();
}

void Player::fall(const float& dt)
{
	this->move(0.f, 2.f, dt);
}
void Player::setVelocityX(const float& x)
{
	this->movementComponent.setVelocityVector(x,0);
}
void Player::setVelocityY( const float& y)
{
	this->movementComponent.setVelocityVector(0,y);
}

void Player::setJumping(const bool& x) 
{
	this->movementComponent.setJumping(x);
}
void Player::setOnPlatform(const bool& x) 
{
	this->movementComponent.setOnPlatform(x);
}

void Player::update(const float& dt, const sf::Vector2i mouse_pos)
{	
	this->movementComponent.update(dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->move(1.f, 0.f, dt);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->move(-1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->movementComponent.canJump())
		this->jump();

	this->hitbox.setPosition(this->sprite[0].position.x + this->hitboxOffset.x,
		this->sprite[0].position.y + this->hitboxOffset.y);

	if (mouse_pos.x < (this->sprite[0].position.x + this->sprite[1].position.x) / 2)
		this->state = &texture2;
	else if (mouse_pos.x > (this->sprite[0].position.x + this->sprite[1].position.x) / 2)
		this->state = &texture;
		
	//this->animation->update(dt);
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(&this->sprite[0], 4, sf::Quads, this->state);
	target.draw(this->hitbox);
}
