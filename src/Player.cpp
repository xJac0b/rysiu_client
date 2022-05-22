#include "pch.h"
#include "Player.h"

Player::Player(sf::FloatRect&& bounds, sf::FloatRect&& hitbox_bounds, sf::String texture_path, sf::Vector2f&& max_velocity,
	float acceleration, float deceleration, sf::String nick, sf::Font&font)
	: Entity(bounds, texture_path, hitbox_bounds, 100),
	movementComponent(sprite, max_velocity.x, acceleration, deceleration, hitboxOffset),
	weapon(sf::FloatRect( 0.f,0.f,100.f,36.f), "Resources/Images/ak.png", 100.f,90.f,17.f)
{
	this->texture2.loadFromFile(texture_path.substring(0, texture_path.find(".")) + "a.png");
	gui::setVertexShape(this->sprite, sf::FloatRect(0.f, 0.f,
		200.f, 200.f), 0);
	this->nick.setString(nick);
	this->nick.setFont(font);
	this->nick.setCharacterSize(16);
}

Player::~Player()
{
}

void Player::move(const float&& dir_x, const float&& dir_y, const float& dt)
{
	this->movementComponent.move(dir_x, dir_y, dt);
}

void Player::jump(const float& dt)
{
	this->movementComponent.jump(dt);
}


void Player::update(const float& dt, const sf::Vector2i mouse_pos)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->move(1.f, 0.f, dt);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->move(-1.f, 0.f, dt);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->movementComponent.canJump())
		this->jump(dt);
	this->movementComponent.update(dt);

	this->hitbox.setPosition(this->sprite[0].position.x + this->hitboxOffset.x,
		this->sprite[0].position.y + this->hitboxOffset.y);

	if (mouse_pos.x < (this->sprite[0].position.x + this->sprite[1].position.x) / 2)
	{
		this->state = &texture2;
		this->weapon.setDirection(false);
		this->rightDirection = false;
	}
	else if (mouse_pos.x > (this->sprite[0].position.x + this->sprite[1].position.x) / 2)
	{
		this->state = &texture;
		this->weapon.setDirection();
		this->rightDirection = true;
	}
		

	this->weapon.update(dt, mouse_pos, { this->getPosition().x + this->getBounds().width/2.f,
		this->getPosition().y + this->getBounds().height /2.f});
	//this->animation->update(dt);
	this->hpBarBack.setPosition(this->getPosition().x, this->getPosition().y-10);
	this->hpBar.setPosition(this->hpBarBack.getPosition().x + 1, this->hpBarBack.getPosition().y + 1);
	
	this->nick.setPosition({ this->getPosition().x + 50 - this->nick.getGlobalBounds().width / 2.f, this->getPosition().y - 30 });
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(this->hpBarBack);
	target.draw(this->hpBar);
	target.draw(&this->sprite[0], 4, sf::Quads, this->state);
	target.draw(this->nick);
	//target.draw(this->hitbox);
	this->weapon.render(target);
}

void Player::onPlatform(bool x)
{
	this->movementComponent.onPlatform(x);
}

void Player::correctBullets(std::vector<sf::Vector2f>to_stay)
{
	this->weapon.correctBullets(to_stay);
}

const bool& Player::rightDir()
{
	return this->rightDirection;
}

const bool& Player::falling()
{
	return this->movementComponent.falling();
}

const float& Player::getWeaponRotation()
{
	return this->weapon.getRotation();
}

const std::vector<sf::Vector2f> Player::getBullets()
{
	return this->weapon.getBullets();
}

