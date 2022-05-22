#include "pch.h"
#include "Obstacle.h"

Obstacle::Obstacle(sf::FloatRect&& bounds, sf::FloatRect textureBounds, sf::FloatRect hitboxBounds , sf::String texture_path)
	: Entity(bounds,texture_path, hitboxBounds)
{
	gui::setVertexShape(this->sprite, { 0,0,88,118 }, 0);
}

Obstacle::~Obstacle()
{
}

void Obstacle::update(const float& dt, const sf::Vector2i mouse_pos)
{
}

void Obstacle::render(sf::RenderTarget& target)
{
	target.draw(&this->sprite[0], 4, sf::Quads, this->state);
	//target.draw(this->hitbox);
}
