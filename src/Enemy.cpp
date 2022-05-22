#include "pch.h"
#include "Enemy.h"

Enemy::Enemy(sf::FloatRect&& bounds, sf::FloatRect&& hitbox_bounds, sf::String texture_path, sf::String nick, sf::Font& font)
	: Entity(bounds, texture_path, hitbox_bounds, 100)
{
	this->texture2.loadFromFile(texture_path.substring(0, texture_path.find(".")) + "a.png");
	gui::setVertexShape(this->sprite, sf::FloatRect(0.f, 0.f,
		200.f, 200.f), 0);
	
	this->weaponTexture.loadFromFile("Resources/Images/ak.png");
	this->weaponSprite.setTexture(this->weaponTexture);
	this->nick.setString(nick);
	this->nick.setFont(font);
	this->nick.setCharacterSize(16);
}

Enemy::~Enemy()
{
}

void Enemy::update(const float& dt, const sf::Vector2i mouse_pos)
{
}

void Enemy::update(const sf::Vector2f pos, float rotation, bool right_direction)
{
	//std::cout << pos.x << " " << pos.y << " " << rotation << right_direction << "\n";
	this->setPosition(pos);
	this->weaponSprite.setRotation(rotation);
	this->weaponSprite.setPosition({ this->getPosition().x + this->getBounds().width / 2.f,
		this->getPosition().y + this->getBounds().height / 2.f });
	if (right_direction)
	{
		this->weaponSprite.setScale({ 1,1 });
		this->state = &texture;
	}
	else
	{
		this->weaponSprite.setScale({ 1,-1 });
		this->state = &texture2;
	}
	this->hpBarBack.setPosition(this->getPosition().x, this->getPosition().y - 10);
	this->hpBar.setPosition(this->hpBarBack.getPosition().x + 1, this->hpBarBack.getPosition().y + 1);
	this->nick.setPosition({ this->getPosition().x + 50 - this->nick.getGlobalBounds().width / 2.f, this->getPosition().y - 30 });
}

void Enemy::render(sf::RenderTarget& target)
{
	target.draw(this->hpBarBack);
	target.draw(this->hpBar);
	target.draw(&this->sprite[0], 4, sf::Quads, this->state);
	target.draw(this->nick);
	target.draw(this->weaponSprite);
}
