#include "pch.h"
#include "Entity.h"

Entity::Entity(sf::FloatRect& bounds, sf::String texture_path, sf::FloatRect hitboxBounds, int hp_max) : texture(texture),
hitboxOffset(hitboxBounds.left, hitboxBounds.top), hpMax(hp_max), hp(hp_max)
{
	this->texture.loadFromFile(texture_path);
	this->sprite.resize(4, sf::Vertex());
	gui::setVertexShape(this->sprite, bounds);
	this->state.texture = &texture;
	this->hitbox.setSize(sf::Vector2f(hitboxBounds.width, hitboxBounds.height)); 
	this->hitbox.setPosition(sf::Vector2f(bounds.left + hitboxBounds.left, bounds.top + hitboxBounds.top));
	this->hitbox.setFillColor(sf::Color::Transparent);
	this->hitbox.setOutlineColor(sf::Color::Green);
	this->hitbox.setOutlineThickness(1.f);
	if (hp_max > 0)
	{
		this->hpBarBack.setFillColor(sf::Color(30, 30, 30, 200));
		this->hpBar.setFillColor(sf::Color(255,0,0,100));
		this->hpBarBack.setSize({90.f, 10.f});
		this->hpBar.setSize({ 88.f, 8.f });
	}
}

Entity::~Entity()
{
}

const sf::FloatRect Entity::getBounds(bool hitboxa)
{
	if (hitboxa)
		return this->hitbox.getGlobalBounds();
	return gui::getVertexShape(this->sprite);
}

const sf::Vector2f& Entity::getPosition(bool hitboxa) const
{
	if (hitboxa)
		return this->hitbox.getPosition();
	return this->sprite[0].position;
}

void Entity::setPosition(sf::Vector2f pos)
{
	gui::setVertexShape(this->sprite, sf::FloatRect(pos, sf::Vector2f(gui::getVertexShape(this->sprite).width,
		gui::getVertexShape(this->sprite).height)));
	this->hitbox.setPosition(this->sprite[0].position.x + this->hitboxOffset.x,
		this->sprite[0].position.y + this->hitboxOffset.y);
}

void Entity::setHp(int hp)
{
	this->hpBar.setSize({ 88.f * ((float)hp / hpMax), this->hpBar.getSize().y });
}
